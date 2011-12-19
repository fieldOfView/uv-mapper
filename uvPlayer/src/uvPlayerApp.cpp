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

#include "cinder/Surface.h"
#include "cinder/Text.h"
#include "cinder/Utilities.h"
#include "cinder/ImageIo.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Fbo.h"
#include "cinder/qtime/QuickTime.h"
#include "cinder/qtime/MovieWriter.h"

#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define APP_WIDTH 700
#define APP_HEIGHT 420

class uvPlayerApp : public AppBasic {
public:
	void prepareSettings ( Settings *settings );
	void setup ();
	void update ();
	void draw ();
	
	void resize ( ResizeEvent event );
	void keyDown ( KeyEvent event );
	void fileDrop( FileDropEvent event );	
	
	string argument ( string argumentName, string defaultValue );	

	enum stateType {
		STATE_PLAYING,
		STATE_EXPORTING,
		STATE_PATTERNS
	};
	stateType mState;

	// player
	void loadMovieFile( const string &path );
	void loadMapFile  ( const string &path );
	void loadOverlayFile ( const string &path );
	void defaultMap   ();
	void defaultImage ();
	void infoTexture  ( const string &title );

	bool			mShowInfo;
	gl::Texture		mFrameTexture, mInfoTexture, mMapTexture, mOverlayTexture;
	qtime::MovieGl	mMovie;
	gl::GlslProg	mShader;

	// exporting
	gl::Fbo				mRenderBuffer;
	qtime::MovieWriter	mMovieWriter;

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

	mOverlayTexture.reset();

	mState = STATE_PLAYING;
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
		if(mState == STATE_EXPORTING) {
			mState = STATE_PLAYING;
			if(mMovie) {
				mMovieWriter.finish();
				mMovie.seekToStart();
				mMovie.play();
			}

		} else {
			if(isFullScreen())
				setFullScreen( false );
			else
				AppBasic::quit();
		}			
		break;
		
	case KeyEvent::KEY_f:
		setFullScreen( ! isFullScreen() );
		
		if(isFullScreen()) {
			hideCursor();
			mShowInfo = false;
		} else
			showCursor();
		

		break;

	case KeyEvent::KEY_i:
		mShowInfo = !mShowInfo;
			
		break;

	case KeyEvent::KEY_SPACE:
		if(mState == STATE_PLAYING) {
			if(mMovie.isPlaying())
				mMovie.stop();
			else
				mMovie.play();
		}
		break;

	case KeyEvent::KEY_BACKSPACE:
		if(mState == STATE_PLAYING) 
			mMovie.seekToStart();
		
		break;

	case KeyEvent::KEY_o:
		// _Open movie/image
		setFullScreen(false);
		path = getOpenFilePath();
		if( !path.empty() ) 
			loadMovieFile( path );
		
		break;

	case KeyEvent::KEY_m:
		// Open _map
		setFullScreen(false);
		path = getOpenFilePath();
		if( !path.empty() ) 
			loadMapFile( path );
		
		break;

	case KeyEvent::KEY_l:
		// Open over_lay
		setFullScreen(false);
		path = getOpenFilePath();
		if( !path.empty() ) 
			loadOverlayFile( path );
		else 
			mOverlayTexture.reset();
		
		break;

	case KeyEvent::KEY_e:
		// Export movie
		switch( mState ) {
		case STATE_PLAYING:
			{
			setFullScreen(false);
			
			path = getSaveFilePath();
			if( path.empty() ) 
				break;
				
			qtime::MovieWriter::Format qtFormat;
			if( !(qtime::MovieWriter::getUserCompressionSettings( &qtFormat, loadImage( loadResource( RES_DEFAULT_IMAGE ) ) ) ) )
				break;
			mMovieWriter = qtime::MovieWriter( path, mMapTexture.getWidth(), mMapTexture.getHeight(), qtFormat );
			
			gl::Fbo::Format renderFormat;
			mRenderBuffer = gl::Fbo( mMapTexture.getWidth(), mMapTexture.getHeight(), renderFormat );

			mMovie.stop();
			mMovie.seekToStart();
			mState = STATE_EXPORTING;

			break;
			}
		case STATE_EXPORTING:
			mState = STATE_PLAYING;
			mMovieWriter.finish();
			mMovie.seekToStart();
			mMovie.play();
			break;
		}

		break;

	case KeyEvent::KEY_p:
		switch( mState ) {
		case STATE_PLAYING:
			if(mMovie) 
				mMovie.stop();			

			mState = STATE_PATTERNS;

			mGraycodeLine = 0;
			mPatternAxis = false;

			break;
		case STATE_PATTERNS:
			mState = STATE_PLAYING;

			if(mMovie) { 
				mMovie.seekToStart();
				mMovie.play();
			}

			break;
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
	switch( mState ) {
	case STATE_PLAYING:
		if( mMovie && mMovie.checkNewFrame() ) {
			mFrameTexture = mMovie.getTexture();
		}
		break;
	case STATE_EXPORTING:
		
		if( !( mMovie.getCurrentTime() >= mMovie.getDuration() - ( 1 / mMovie.getFramerate() ) ) ) {
			mMovie.stepForward();
			mFrameTexture = mMovie.getTexture();
		} else {
			mMovie.seekToStart();
			mMovie.play();

			mMovieWriter.finish();
			mFrameTexture.reset();

			mState = STATE_PLAYING;
		}
	}
}

void uvPlayerApp::draw()
{
	gl::enableAlphaBlending();
	gl::clear( Color( 0, 0, 0 ) );
	
	switch( mState ) {
	case STATE_PLAYING:
	case STATE_EXPORTING:

		if( mFrameTexture && mMapTexture ) {
			Rectf centeredRect;

			if( mState == STATE_EXPORTING ) {
				// this will restore the old framebuffer binding when we leave this function
				// on non-OpenGL ES platforms, you can just call mFbo.unbindFramebuffer() at the end of the function
				// but this will restore the "screen" FBO on OpenGL ES, and does the right thing on both platforms
				gl::SaveFramebufferBinding bindingSaver;
	
				// bind the framebuffer - now everything we draw will go there
				mRenderBuffer.bindFramebuffer();
				//gl::clear( Color(255, 0, 0) );

				centeredRect = Rectf( mRenderBuffer.getBounds() );
				gl::setViewport( mRenderBuffer.getBounds() );

			} else {
				centeredRect = Rectf( mMapTexture.getBounds() ).getCenteredFit( getWindowBounds(), true );
				gl::setViewport( getWindowBounds() );
			}
			
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
			
			if( mOverlayTexture ) {
				gl::draw( mOverlayTexture, centeredRect );
			}

			if( mState == STATE_EXPORTING ) {
				mRenderBuffer.unbindFramebuffer();
				glEnable( GL_TEXTURE_2D );

				Surface processedFrame = Surface(mRenderBuffer.getTexture() );
				mMovieWriter.addFrame( processedFrame );

				gl::setViewport( getWindowBounds() );

				gl::draw( processedFrame, mRenderBuffer.getBounds(), Rectf( mRenderBuffer.getBounds() ).getCenteredFit( getWindowBounds(), true ));
			}
		}

		if( mInfoTexture && mShowInfo ) {
			glDisable( GL_TEXTURE_RECTANGLE_ARB );
			gl::draw( mInfoTexture, Vec2f( 20, getWindowHeight() - 20 - (float)mInfoTexture.getHeight() ) );
		}

		break;
		
	case STATE_PATTERNS:

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
		if( mOverlayTexture ) {
			gl::draw( mOverlayTexture, getWindowBounds() );
		}

		break;
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

void uvPlayerApp::loadOverlayFile( const string &overlayPath )
{
	try {
		mOverlayTexture = gl::Texture( loadImage( overlayPath ) );
	}
	catch( ... ) {
		console() << "Unable to load overlay file." << endl;
		
		mOverlayTexture.reset();
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
		if( mMovie.getNumFrames() > 0 ) {
			infoText.addLine( toString( mMovie.getDuration() ) + " seconds" );
			infoText.addLine( toString( mMovie.getNumFrames() ) + " frames" );
			infoText.addLine( toString( mMovie.getFramerate() ) + " fps" );
		} else {
			infoText.addLine( "still image" );
		}
	}

	infoText.addCenteredLine( "Keys" );
	infoText.addLine( "space: play/pause" );
	infoText.addLine( "backspace: rewind" );
	infoText.addLine( "o: load movie" );
	infoText.addLine( "m: load uvmap" );
	infoText.addLine( "l: load overlay" );
	infoText.addLine( "e: export processed movie" );
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

CINDER_APP_BASIC( uvPlayerApp, RendererGl(RendererGl::AA_NONE) );