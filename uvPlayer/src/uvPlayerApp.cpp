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

class uvPlayerApp : public AppBasic {
public:
	void setup();
	void update();
	void draw();
	
	void keyDown ( KeyEvent event );
	void fileDrop( FileDropEvent event );	
	
	string argument     ( string argumentName, string defaultValue );	
	void   loadMovieFile( const string &path );
	void   loadMapFile  ( const string &path );
	void   defaultMap   ();
	
	bool			mShowInfo;
	gl::Texture		mFrameTexture, mInfoTexture, mMapTexture;
	qtime::MovieGl	mMovie;
	gl::GlslProg	mShader;
};

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
	if( moviePath.empty() )
		moviePath = getOpenFilePath();
	
	if( ! moviePath.empty() )
		loadMovieFile( moviePath );
	
	
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
			setFullScreen( ! isFullScreen() );
			
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

	}
}

void uvPlayerApp::fileDrop( FileDropEvent event )
{
	loadMovieFile( event.getFile( 0 ) );
}

void uvPlayerApp::update()
{
	if( mMovie )
		mFrameTexture = mMovie.getTexture();
}

void uvPlayerApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
	gl::enableAlphaBlending();
	
	if( mFrameTexture && mMapTexture ) {
		Rectf centeredRect = Rectf( mMapTexture.getBounds() ).getCenteredFit( getWindowBounds(), true );
		
		mShader.bind();

		mMapTexture.bind( 0 );
		mShader.uniform( "map", 0 );
		
		mFrameTexture.enableAndBind();
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
		
		// create a texture for showing some info about the movie
		TextLayout infoText;
		infoText.clear( ColorA( 0.2f, 0.2f, 0.2f, 0.5f ) );
		infoText.setColor( Color::white() );
		infoText.addCenteredLine( getPathFileName( moviePath ) );
		infoText.addLine( toString( mMovie.getWidth() ) + " x " + toString( mMovie.getHeight() ) + " pixels" );
		infoText.addLine( toString( mMovie.getDuration() ) + " seconds" );
		infoText.addLine( toString( mMovie.getNumFrames() ) + " frames" );
		infoText.addLine( toString( mMovie.getFramerate() ) + " fps" );
		
		infoText.addCenteredLine( "Keys" );
		infoText.addLine( "o: load movie" );
		infoText.addLine( "m: load uvmap" );
		infoText.addLine( "f: toggle fullscreen" );
		infoText.addLine( "i: toggle info" );

		infoText.setBorder( 4, 2 );
		mInfoTexture = gl::Texture( infoText.render( true ) );
	}
	catch( ... ) {
		console() << "Unable to load the movie." << std::endl;
		mMovie.reset();
		mInfoTexture.reset();
	}
	
	mFrameTexture.reset();
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
	
	mFrameTexture.reset();
}

void uvPlayerApp::defaultMap()
{
	Surface16u defaultMap = Surface16u( 640, 480, false, SurfaceChannelOrder::RGB );
	Surface16u::Iter defaultMapIter( defaultMap.getIter() );
	
	while ( defaultMapIter.line() ) {
		uint16_t vValue = (uint16_t)(( defaultMapIter.y() << 16 ) / 480 );
		while ( defaultMapIter.pixel() ) {
			defaultMapIter.r() = (uint16_t)(( defaultMapIter.x() << 16 ) / 640 );
			defaultMapIter.g() = vValue;
		}
	}

	mMapTexture = gl::Texture( defaultMap );
}

CINDER_APP_BASIC( uvPlayerApp, RendererGl );
