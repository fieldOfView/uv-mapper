/*
 Copyright (c) Aldo Hoeben / fieldOfView
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#include "cinder/app/AppBasic.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTime.h"
#include "cinder/Text.h"
#include "cinder/Utilities.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/GlslProg.h"

#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define APP_WIDTH 700
#define APP_HEIGHT 420

class uvPlayerApp : public AppBasic {
public:
	void prepareSettings( Settings *settings );
	void setup();
	void update();
	void draw();
	
	void resize( ResizeEvent event );
	void keyDown ( KeyEvent event );
	void fileDrop( FileDropEvent event );	
	
	bool			mShowPatterns;

	// player
	string argument     ( string argumentName, string defaultValue );	
	void   loadMovieFile( const string &path );
	void   loadMapFile  ( const string &path );
	void   defaultMap   ();
	void   defaultImage ();
	void   infoTexture  ( const string &title );

	bool			mShowInfo;
	gl::Texture		mFrameTexture, mInfoTexture, mMapTexture;
	qtime::MovieGl	mMovie;
	gl::GlslProg	mShader;

	// patterns
	Channel graycodes ( int aBits );

	Channel mGraycode;
	gl::Texture mPatternTexture;

	int mGraycodeLine;
	bool mPatternAxis;
};

void uvPlayerApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize(APP_WIDTH, APP_HEIGHT);
	settings->setTitle("uvPlayer");

	const vector<shared_ptr<Display> > &displays( Display::getDisplays() );
	for( vector<shared_ptr<Display> >::const_iterator it = displays.begin(); it != displays.end(); ++it ) {
		Area a = (*it)->getArea();
		console() << "Display: " << a << " size: " << a.getWidth() << " x " << a.getHeight() << " @ depth: " << (*it)->getBitsPerPixel() << endl;
	}

	if( displays.size() > 1 )
		settings->setDisplay( displays[1] );
}

void uvPlayerApp::setup()
{
	try {
		mShader = gl::GlslProg( loadResource( RES_PASSTHRU_VERT ), loadResource( RES_EFFECT_FRAG ) );
	}
	catch( gl::GlslProgCompileExc &exc ) {
		console() << "Shader compile error: " << endl;
		console() << exc.what();
		AppBasic::quit();
		return;
	}
	catch( ... ) {
		console() << "Unable to load shader" << endl;
		AppBasic::quit();
		return;
	}
	
	string mapPath = argument("map","");
	if( ! mapPath.empty() ) 
		loadMapFile( mapPath );
	else
		defaultMap();
	
	string moviePath = argument("file","");
	if( ! moviePath.empty() )
		loadMovieFile( moviePath );
	else
		defaultImage();

	mShowPatterns = false;
}

void uvPlayerApp::resize( ResizeEvent event )
{
	int size = max( getWindowWidth(), getWindowHeight() );
	int bits = (int)ceil( log( (double)size ) / log( (double)2 ) );
	mGraycode = graycodes( bits );

	mPatternTexture = gl::Texture( mGraycode );
	mPatternTexture.setMagFilter( GL_NEAREST );

	mGraycodeLine = 0;
	mPatternAxis = false;
}

void uvPlayerApp::keyDown( KeyEvent event )
{
	string path;

	switch ( event.getCode() ) {
	case KeyEvent::KEY_ESCAPE:
		if(isFullScreen())
			setFullScreen( false );
		else
			AppBasic::quit();
			
		break;
		
	case KeyEvent::KEY_f:
		// todo: go to full screen on the screen the window is on 
		setFullScreen( ! isFullScreen() );
		// todo: fix hiding cursor after going fullscreen
		
		if(isFullScreen()) {
			hideCursor();
			mShowInfo = false;
		} else
			showCursor();
		

		break;

	case KeyEvent::KEY_i:
		mShowInfo = !mShowInfo;
			
		break;
		
	case KeyEvent::KEY_o:
		setFullScreen(false);
		path = getOpenFilePath();
		if( !path.empty() ) {
			loadMovieFile( path );
		}
		break;

	case KeyEvent::KEY_m:
		setFullScreen(false);
		path = getOpenFilePath();
		if( !path.empty() ) {
			loadMapFile( path );
		}
		break;

	case KeyEvent::KEY_p:
		mShowPatterns = !mShowPatterns;
		
		if(mShowPatterns) {
			mGraycodeLine = 0;
			mPatternAxis = false;

			if(mMovie) 
				mMovie.stop();
		} else {
			if(mMovie) { 
				mMovie.seekToStart();
				mMovie.play();
			}
		}

		break;

	case KeyEvent::KEY_LEFT:
		if ( mGraycodeLine > 0 )
			mGraycodeLine--;
		else {
			mGraycodeLine = mGraycode.getHeight() - 1;
			mPatternAxis = !mPatternAxis;
		}

		break;

	case KeyEvent::KEY_RIGHT:
		if ( mGraycodeLine < ( mGraycode.getHeight() - 1 ) ) 
			mGraycodeLine++;
		else {
			mGraycodeLine = 0;
			mPatternAxis = !mPatternAxis;
		}

		break;

	}
}

void uvPlayerApp::fileDrop( FileDropEvent event )
{
	loadMovieFile( event.getFile( 0 ) );
}

void uvPlayerApp::update()
{
	if( mMovie && mMovie.checkNewFrame() ) {
		mFrameTexture = mMovie.getTexture();
	}
}

void uvPlayerApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
	
	if( !mShowPatterns ) {
		gl::enableAlphaBlending();
	
		if( mFrameTexture && mMapTexture ) {
			Rectf centeredRect = Rectf( mMapTexture.getBounds() ).getCenteredFit( getWindowBounds(), true );

			mShader.bind();

			mMapTexture.bind( 0 );
			mShader.uniform( "map", 0 );

			mFrameTexture.bind( 1 );
			mShader.uniform( "frame", 1 );
			mShader.uniform( "frameSize", Vec2f( (float)mFrameTexture.getWidth(), (float)mFrameTexture.getHeight() ) );
			mShader.uniform( "flipv", mFrameTexture.isFlipped() );
		
			gl::drawSolidRect( centeredRect );
		
			mFrameTexture.unbind();
			mMapTexture.unbind();
			mShader.unbind();
		}
	
		if( mInfoTexture && mShowInfo ) {
			glDisable( GL_TEXTURE_RECTANGLE_ARB );
			gl::draw( mInfoTexture, Vec2f( 20, getWindowHeight() - 20 - (float)mInfoTexture.getHeight() ) );
		}
	} else {
		int32_t patternWidth = mGraycode.getWidth();

		int32_t offset = 0;

		if(mPatternTexture) {
			if( ! mPatternAxis ) {
				offset = ( patternWidth - getWindowWidth() ) / 2;

				gl::translate( Vec2f( (float)-offset, 0 ) );
			
				gl::draw( mPatternTexture, Area( 0, mGraycodeLine, mGraycode.getWidth(), mGraycodeLine+1 ), Area( 0, 0, patternWidth, patternWidth ) ); 
			
				gl::translate( Vec2f( (float)offset, 0 ) );
			} else {
				offset = ( patternWidth - getWindowHeight() ) / 2;

				gl::translate( Vec2f( (float)patternWidth , (float)-offset ) );
				gl::rotate( 90.0 );

				gl::draw( mPatternTexture, Area( 0, mGraycodeLine, mGraycode.getWidth(), mGraycodeLine+1 ), Area( 0, 0, patternWidth, patternWidth ) ); 

				gl::rotate( -90.0 );
				gl::translate( Vec2f( -(float)patternWidth , (float)offset ) );
			}
		}
	}
}

string uvPlayerApp::argument(string argumentName, string defaultValue = "")
{
	// utility method: find argument from commandline arguments
	for( vector<string>::const_iterator argIter = getArgs().begin(); argIter != getArgs().end(); ++argIter ) {
		if(("-"+argumentName) == *argIter && argIter != getArgs().end()) {
			++argIter;
			return *argIter;
		}
	}
	return defaultValue;
}

void uvPlayerApp::loadMovieFile( const string &moviePath )
{
	try {
		// load up the movie, set it to loop, and begin playing
		mMovie = qtime::MovieGl( moviePath );
		mMovie.setLoop();
		mMovie.play();

		infoTexture( getPathFileName( moviePath ) );
	}
	catch( ... ) {
		console() << "Unable to load the movie." << endl;
		mMovie.reset();
		defaultImage();
	}
	
	mFrameTexture.reset();
}

void uvPlayerApp::defaultImage()
{
	gl::Texture::Format format;
	format.setTargetRect();

	mFrameTexture = gl::Texture( loadImage( loadResource( RES_DEFAULT_IMAGE ) ), format );

	infoTexture( "No movie loaded" );
}

void uvPlayerApp::loadMapFile( const string &mapPath )
{
	try {
		mMapTexture = gl::Texture( loadImage( mapPath ) );
	}
	catch( ... ) {
		console() << "Unable to load uv map file." << endl;
		
		defaultMap();
	};
}

void uvPlayerApp::defaultMap()
{
	Surface16u defaultMap = Surface16u( APP_WIDTH, APP_HEIGHT, false, SurfaceChannelOrder::RGB );
	Surface16u::Iter defaultMapIter( defaultMap.getIter() );
	
	while ( defaultMapIter.line() ) {
		uint16_t vValue = (uint16_t)(( defaultMapIter.y() << 16 ) / APP_HEIGHT );
		while ( defaultMapIter.pixel() ) {
			defaultMapIter.r() = (uint16_t)(( defaultMapIter.x() << 16 ) / APP_WIDTH );
			defaultMapIter.g() = vValue;
		}
	}

	mMapTexture = gl::Texture( defaultMap );
}

void uvPlayerApp::infoTexture( const string &title )
{
	// create a texture for showing some info about the movie
	TextLayout infoText;
	infoText.clear( ColorA( 0.2f, 0.2f, 0.2f, 0.5f ) );
	infoText.setColor( Color::white() );
	infoText.addCenteredLine( title );
	if( mMovie ) {
		infoText.addLine( toString( mMovie.getWidth() ) + " x " + toString( mMovie.getHeight() ) + " pixels" );
		infoText.addLine( toString( mMovie.getDuration() ) + " seconds" );
		infoText.addLine( toString( mMovie.getNumFrames() ) + " frames" );
		infoText.addLine( toString( mMovie.getFramerate() ) + " fps" );
	}

	infoText.addCenteredLine( "Keys" );
	infoText.addLine( "o: load movie" );
	infoText.addLine( "m: load uvmap" );
	infoText.addLine( "f: toggle fullscreen" );
	infoText.addLine( "i: toggle info" );
	infoText.addLine( "p: toggle graycode patterns" );

	infoText.setBorder( 4, 2 );
	mInfoTexture = gl::Texture( infoText.render( true ) );
}

Channel uvPlayerApp::graycodes( int aBits ) 
{
	Channel graycodes( (int32_t)pow( (double)2, aBits ), (int32_t)aBits + 1 ); 
	Channel::Iter graycodesIter( graycodes.getIter() );

	uint8_t value;
	int chunkWidth;
	int count;

	while ( graycodesIter.line() ) {
		if ( graycodesIter.y() < aBits ) {
			value = 0;
			
			chunkWidth = (int)pow( (double)2, aBits - graycodesIter.y() );
			count = chunkWidth / 2;
			
			while ( graycodesIter.pixel() ) {
				graycodesIter.v() = value;

				count++;
				if ( count >= chunkWidth ) {
					value = (value == 0) ? -1 : 0;
					count = 0;
				}
			}
		} else {
			while( graycodesIter.pixel() ) {
				graycodesIter.v() = -1 - graycodesIter.v(0, -( aBits - 4 ) );
			}
		}
	}
	return graycodes;
}

CINDER_APP_BASIC( uvPlayerApp, RendererGl );
