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
#include "cinder/Utilities.h"
#include "cinder/params/Params.h"

#include "cinder/ImageIo.h"
#include "cinder/Surface.h"
#include "cinder/ip/Fill.h"
#include "cinder/ip/Grayscale.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/texture.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Fbo.h"

#include "cinder/qtime/QuickTime.h"

#include "uvToolResources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define APP_WIDTH 860
#define APP_HEIGHT 540

class uvToolApp : public AppBasic {
public:
	void prepareSettings ( Settings *settings );
	
	void setup();
	void update();
	void draw();

	void resize ( ResizeEvent event );
	void keyDown ( KeyEvent event );

private:
	void openFile ( bool askFilename );
	void saveFile ( bool askFilename );
	void switchUndo();
	void storeUndo();

	void passthroughMap();
	void mapFromPatterns();
	void inverseMap();

	enum UVFILTERS {
		FILTER_MEDIAN,
		FILTER_GAUSSIAN,
		FILTER_DESPECKLE
	};
	void applyFilter( UVFILTERS filterType );
	void fillHoles();

	void toggleFullscreen();

	// map display
	enum UVTEXTURES {
		TEXTURE_MAP,
		TEXTURE_ALPHA,
		TEXTURE_GRID,
		TEXTURE_MOVIE
	};
	void setTexture( UVTEXTURES textureType );
	void updateTexture();
	void makeGrid();

	gl::Texture		mTexture;
	gl::GlslProg	mUVShader;
	qtime::MovieGl	mMovie;
	gl::Texture		mGridTexture;
	gl::Fbo			mRenderBuffer;

	// ui
	params::InterfaceGl	mParams;
	bool mShowParams;

	gl::Texture mDisplayTexture;
	UVTEXTURES mDisplayType;
	
	// map
	Surface16u mMap;
	fs::path mFilename;
	Surface16u mUndoMap;

	int32_t mMapWidth;
	int32_t mMapHeight;
	int32_t mMapBits;

	// create from patterns
	int32_t mPatternAsymmetric;	
	int32_t mPatternContrast;
	int mPatternChannel;

	// filter
	int32_t mFilterRadius;
	int32_t mFilterThreshold;

	// utility
	void bindFbo( gl::Fbo buffer );
	void unbindFbo( gl::Fbo buffer );
};

void uvToolApp::prepareSettings( Settings *settings ) 
{
	settings->setWindowSize(APP_WIDTH, APP_HEIGHT);
	settings->setTitle("UV Mapper Tool");
}


void uvToolApp::setup()
{
	// Load and compile UV Mapping Shader
	try {
		mUVShader = gl::GlslProg( loadResource( RES_PASSTHRU_VERT ), loadResource( RES_UVMAP_FRAG ) );
	}
	catch( gl::GlslProgCompileExc &exc ) {
		console() << "Shader compile error: " << endl;
		console() << exc.what();
		uvToolApp::quit();
		return;
	}
	catch( ... ) {
		console() << "Unable to load shader" << endl;
		uvToolApp::quit();
		return;
	}


	mMapWidth = uvToolApp::getSettings().getDisplay()->getWidth();
	mMapHeight = uvToolApp::getSettings().getDisplay()->getHeight();
	mMapBits = (int32_t)ceil( log( (double)max(mMapWidth, mMapHeight) ) / log( 2. ) );

	// create empty document
	passthroughMap();

	mPatternAsymmetric = 2;	
	mPatternContrast = 8;
	mPatternChannel = 0;

	mFilterRadius = 1;
	mFilterThreshold = 0;

	// initialise display
	makeGrid();
	setTexture( TEXTURE_MAP );

	// initialise params interface
	mShowParams = true;

	mParams = params::InterfaceGl( "UV Mapper Tool", Vec2i( 200, 400 )  );
	mParams.setOptions ( "", "size=`200 380` position=`10 10` fontresizable=false resizable=false movable=false help=`This application creates and edits UV Maps to distort 2d content.`" );
	
	mParams.addButton ( "file_open",	std::bind( &uvToolApp::openFile, this, true ),	"label='Open...' group=file key=CTRL+o" );
	mParams.addButton ( "file_reopen",	std::bind( &uvToolApp::openFile, this, false ),	"label='Reload' group=file key=CTRL+r" );
	mParams.addButton ( "file_save",	std::bind( &uvToolApp::saveFile, this, false ),	"label='Save' group=file key=CTRL+s" );
	mParams.addButton ( "file_saveas",	std::bind( &uvToolApp::saveFile, this, true ),	"label='Save as...' group=file key=CTRL+S" );
	mParams.addSeparator ( "file_sep1",		"group=file" );
	mParams.addButton ( "file_undo",	std::bind( &uvToolApp::switchUndo, this ),		"label='Undo' group=file key=CTRL+z" );
	mParams.setOptions ( "file",		"label='File'" );

	mParams.addButton ( "create_passthrough",	std::bind( &uvToolApp::passthroughMap, this ),	"label='Unit map' group=create key=CTRL+n" );
	mParams.addButton ( "create_frompatterns",	std::bind( &uvToolApp::mapFromPatterns, this ),	"label='From patterns...' group=create key=CTRL+p" );
	mParams.addButton ( "create_inverse",		std::bind( &uvToolApp::inverseMap, this ),		"label='Inverse map' group=create key=CTRL+i" );
	mParams.setOptions ( "create",				"label='Create'" );
	mParams.addParam ( "create_width",			&mMapWidth,		"label='Width' group=create_settings min=16 max=4096" );
	mParams.addParam ( "create_height",			&mMapHeight,	"label='Height' group=create_settings min=16 max=4096" );
	mParams.addParam ( "create_bits",			&mMapBits,		"label='Bits' group=create_settings min=4 max=12" );
	mParams.addSeparator ( "create_sep1",		"group=create_settings" );
	vector<string> channelNames;
	channelNames.push_back( "Red" ); channelNames.push_back( "Green" ); channelNames.push_back( "Blue" ); channelNames.push_back( "Grayscale" ); 
	mParams.addParam( "create_channel",			channelNames, &mPatternChannel,"label='Color channel' group=`create_settings`" );
	mParams.addParam( "create_asymmetric",		&mPatternAsymmetric,	"label='Asymmetric patterns' group=create_settings min=0 max=15" );
	mParams.addParam( "create_contrast",		&mPatternContrast,		"label='Recovery contrast' group=create_settings min=1 max=127" );
	mParams.setOptions ( "create_settings",		"label='Settings' group=create opened=false" );

	mParams.addButton ( "filter_median",		std::bind( &uvToolApp::applyFilter, this, FILTER_MEDIAN ),	"label='Median' group=filter" );
	mParams.addButton ( "filter_gaussian",		std::bind( &uvToolApp::applyFilter, this, FILTER_GAUSSIAN ),	"label='Gaussian' group=filter" );
	mParams.addButton ( "filter_despeckle",		std::bind( &uvToolApp::applyFilter, this, FILTER_DESPECKLE ), "label='Despeckle' group=filter" );
	mParams.addButton ( "filter_fillholes",		std::bind( &uvToolApp::fillHoles, this ),		"label='Fill holes' group=filter" );
	mParams.setOptions ( "filter",		"label='Filter'" );
	mParams.addParam ( "filter_radius",			&mFilterRadius,		"label='Radius' group=filter_settings min=1 max=64" );
	mParams.addParam ( "filter_threhold",		&mFilterThreshold,	"label='Threshold' group=filter_settings min=0 max=255" );
	mParams.setOptions ( "filter_settings",		"label='Settings' group=filter opened=false" );	

	mParams.addButton ( "view_fullscreen",		std::bind( &uvToolApp::toggleFullscreen, this ), "label='Full screen' group=view key=CTRL+f" );
	mParams.addSeparator ( "view_sep1",			"group=view" );
	mParams.addButton ( "view_map",				std::bind( &uvToolApp::setTexture, this, TEXTURE_MAP ), "label='Map' group=view" );
	mParams.addButton ( "view_alpha",				std::bind( &uvToolApp::setTexture, this, TEXTURE_ALPHA ), "label='Alpha' group=view" );
	mParams.addButton ( "view_grid",			std::bind( &uvToolApp::setTexture, this, TEXTURE_GRID ), "label='Grid' group=view" );
	mParams.addButton ( "view_movie",			std::bind( &uvToolApp::setTexture, this, TEXTURE_MOVIE ), "label='File...' group=view" );
	mParams.setOptions ( "view",		"label='View'" );

}


void uvToolApp::update()
{
	updateTexture();
}

void uvToolApp::draw()
{
    gl::enableAlphaBlending();

	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 

	// draw display/preview texture, if any
	if( mDisplayTexture )
		gl::draw( mDisplayTexture, Rectf( mDisplayTexture.getBounds() ).getCenteredFit( getWindowBounds(), true ) );

	// draw params interface
	if( mShowParams )
		mParams.draw();
}

void uvToolApp::resize ( ResizeEvent event )
{
	string optString = ((string)"size=`200 ").append( toString( event.getHeight() - 40 ) ).append("`");
	mParams.setOptions ( "", optString );
}

void uvToolApp::keyDown ( KeyEvent event )
{
	// NB: most key events are handled by Params interface

	const int keyCode = event.getCode();

	// Cmd + Q and Alt + F4
	if( (event.isControlDown() && keyCode == KeyEvent::KEY_q) ||
		(event.isAltDown() && keyCode == KeyEvent::KEY_F4) ) {
		uvToolApp::quit();
		return;
	}

	// Esc aborts current operation, exits fullscreen or closes app
	if( keyCode == KeyEvent::KEY_ESCAPE ) {
		// TODO: abort current operation, or:
		if( isFullScreen() ) 
			setFullScreen( false );
		else
			uvToolApp::quit();

		return;
	}

	if( keyCode == KeyEvent::KEY_TAB ) {
		mShowParams = !mShowParams;
	}
}

void uvToolApp::openFile( bool askFilename ) 
{
	if( askFilename || mFilename.empty() ) {
		fs::path filename = getOpenFilePath();
		if ( !filename.empty() )
			mFilename = filename;
		else
			return;
	}

	try {
		mMap = Surface16u( loadImage( mFilename ) );
	}
	catch( ... ) {
		console() << "Unable to load uv map file." << endl;
		return;
	};

	storeUndo();
	mRenderBuffer = gl::Fbo( mMap.getWidth(), mMap.getHeight(), mMap.hasAlpha() );
	mDisplayTexture.reset();

	setTexture( TEXTURE_MAP );
}

void uvToolApp::saveFile( bool askFilename ) 
{
	if( askFilename || mFilename.empty() ) {
		fs::path filename = getSaveFilePath();
		if ( filename.empty() )
			return;

		mFilename = filename;
		mFilename.replace_extension( ".tif" ); 
	}

	try {
		writeImage( mFilename, mMap );
	}
	catch( ... ) {
		console() << "Unable to save uv map file." << endl;
		return;
	};
	storeUndo();
}

void uvToolApp::storeUndo()
{
	mUndoMap = mMap.clone();
}

void uvToolApp::switchUndo()
{
	Surface16u undo = mMap.clone();
	mMap = mUndoMap;
	mUndoMap = undo;

	mRenderBuffer = gl::Fbo( mMap.getWidth(), mMap.getHeight(), mMap.hasAlpha() );
	mDisplayTexture.reset();
}

void uvToolApp::passthroughMap()
{
	mMap = Surface16u( mMapWidth, mMapHeight, false, SurfaceChannelOrder::RGB );
	ip::fill( &mMap, Color(0, 0, 0) );

	Surface16u::Iter mapIter( mMap.getIter() );

	while ( mapIter.line() ) {
		uint16_t vValue = (uint16_t)(( mapIter.y() << 16 ) / mMapHeight );
		while ( mapIter.pixel() ) {
			mapIter.r() = (uint16_t)(( mapIter.x() << 16 ) / mMapWidth );
			mapIter.g() = vValue;
			mapIter.b() = 0;
		}
	}
	mFilename.clear();

	storeUndo();
	mRenderBuffer = gl::Fbo( mMap.getWidth(), mMap.getHeight(), mMap.hasAlpha() );
	mDisplayTexture.reset();

	setTexture( TEXTURE_MAP );
}

void uvToolApp::mapFromPatterns()
{
	mFilename.clear();
	storeUndo();
	mRenderBuffer = gl::Fbo( mMap.getWidth(), mMap.getHeight(), mMap.hasAlpha() );
	mDisplayTexture.reset();

	setTexture( TEXTURE_MAP );
}

void uvToolApp::inverseMap()
{
	// TODO: make mInverseThreshold into a setting (or factor out mInverseThreshold)
	int32_t mInverseThreshold = 8;
	
	// invert created uv-map
	Surface16u newMap = Surface16u( mMapWidth,mMapHeight, true, SurfaceChannelOrder::RGBA ); 
	ip::fill( &newMap, ColorAT<uint16_t>( 0, 0, 0, 0 ) );
	Channel16u newMapAlpha = newMap.getChannelAlpha();

	Surface16u::Iter mapIter( mMap.getIter() );
	
	unsigned int mapWidth = mMap.getWidth() - 1;
	unsigned int mapHeight = mMap.getHeight() - 1;
	
	unsigned int patternWidth = (unsigned int)pow( 2., (double)mMapBits );

	double coordMultiplier = (double)patternWidth / 65536;
	int xOffset = ( patternWidth - mMapWidth) / 2;
	int yOffset = ( patternWidth - mMapHeight) / 2;

	bool mapHasAlpha = mMap.hasAlpha();

	while ( mapIter.line() ) {
		while ( mapIter.pixel() ) {
			if ( ( !mapHasAlpha || mapIter.a() > mInverseThreshold ) && mapIter.r() > 0 && mapIter.g() > 0 ) {
				// multiply u&v to cover 16 bit resolution
				Vec2i mappedCoordinate = Vec2i( (int)( coordMultiplier * mapIter.r() - xOffset ), (int)( coordMultiplier * mapIter.g() - yOffset ) );
				
				if( mapHasAlpha ) {
					// check if a "better defined" pixel in the uvmap has already "claimed" this mapped coordinate
					uint16_t alphaValue = mapIter.a();
					if ( newMapAlpha.getValue( mappedCoordinate ) < alphaValue ) {
						newMap.setPixel( mappedCoordinate, ColorT<uint16_t>(
												(uint16_t)( ( (uint32_t)mapIter.x() << 16) / mapWidth ), 
												(uint16_t)( ( (uint32_t)mapIter.y() << 16) / mapHeight ), 0 ) );
						newMapAlpha.setValue( mappedCoordinate, alphaValue );
					}
				} else {
					newMap.setPixel( mappedCoordinate, ColorT<uint16_t>(
											(uint16_t)( ( (uint32_t)mapIter.x() << 16) / mapWidth ), 
											(uint16_t)( ( (uint32_t)mapIter.y() << 16) / mapHeight ), 0 ) );
					newMapAlpha.setValue( mappedCoordinate, -1 );	
				}
			}
		}	
	}
	
	storeUndo();
	// inverted map is new document
	mMap = newMap;	

	mRenderBuffer = gl::Fbo( mMap.getWidth(), mMap.getHeight(), mMap.hasAlpha() );
	mDisplayTexture.reset();

	setTexture( TEXTURE_MAP );
}

void uvToolApp::applyFilter( UVFILTERS filterType ) 
{
	switch (filterType) {
	case FILTER_MEDIAN:
		break;
	case FILTER_GAUSSIAN:
		break;
	case FILTER_DESPECKLE:
		break;
	}

	storeUndo();
	mDisplayTexture.reset();
}

void uvToolApp::fillHoles()
{
}

void uvToolApp::toggleFullscreen()
{
	bool fullScreen = !isFullScreen();

	setFullScreen( fullScreen );
	mParams.setOptions ( "view_fullscreen",	 ( fullScreen ) ? "label='Exit full screen'" : "label='Full screen'" );
}

void uvToolApp::setTexture( UVTEXTURES textureType ) 
{
	mDisplayTexture.reset();

	switch( textureType ) {
	case TEXTURE_MAP:
		break;

	case TEXTURE_ALPHA:
		break;

	case TEXTURE_GRID:
		mTexture = mGridTexture;
		break;

	case TEXTURE_MOVIE:
		break;
	}
	mDisplayType = textureType;
}

void uvToolApp::updateTexture() 
{
	switch( mDisplayType ) {
	case TEXTURE_MAP:
		if( !mDisplayTexture && mMap )
			mDisplayTexture = gl::Texture( mMap );
		break;

	case TEXTURE_ALPHA:
		if( !mDisplayTexture && mMap && mMap.hasAlpha() ) 
			mDisplayTexture = gl::Texture( mMap.getChannelAlpha().clone() );
		break;

	case TEXTURE_GRID:
		if( !mDisplayTexture ) {
			bindFbo( mRenderBuffer );

			// clear the buffer
			gl::clear( Color( 0, 0, 0 ) );

			if( mTexture ) {
				// use uvmap shader to draw frame into Fbo
				mUVShader.bind();

				gl::Texture mapTexture = gl::Texture( mMap );
				mapTexture.bind( 0 );
				mUVShader.uniform( "map", 0 );

				mTexture.bind( 1 );
				mUVShader.uniform( "frame", 1 );
				mUVShader.uniform( "frameSize", Vec2f( (float)mTexture.getWidth(), (float)mTexture.getHeight() ) );
				mUVShader.uniform( "flipv", mTexture.isFlipped() );
		
				// draw fbo upsidedown because
				gl::drawSolidRect( Rectf ( 0., (float)mRenderBuffer.getHeight(), (float)mRenderBuffer.getWidth(), 0.) );
		
				mTexture.unbind();
				mapTexture.unbind();
				mUVShader.unbind();
			}
			unbindFbo( mRenderBuffer );

			mDisplayTexture = mRenderBuffer.getTexture();
		}
		break;

	case TEXTURE_MOVIE:
		break;

	}
}

void uvToolApp::makeGrid() 
{
	float gridWidth = 1024.;
	float gridHeight = 1024.;

	// create grid texture as texture2drect, no alpha
	gl::Fbo::Format bufferFormat = gl::Fbo::Format();
	bufferFormat.setTarget( GL_TEXTURE_RECTANGLE_ARB );
	bufferFormat.setColorInternalFormat( GL_RGB8 );
	gl::Fbo gridBuffer = gl::Fbo( (int)gridWidth, (int)gridHeight, bufferFormat ); 
	
	bindFbo( gridBuffer );

	// clear the buffer
	gl::clear( Color( 0, 0, 0 ) );
	gl::color( Color( 255, 255, 255 ) );
	// draw grid
	for( float i=1; i< gridWidth; i += 16. ) {
		gl::drawLine( Vec2f( i, 0. ), Vec2f( i, gridHeight ) );
	}
	gl::drawLine( Vec2f( gridWidth, 0. ), Vec2f( gridWidth, gridHeight ) );
	for( float j=1; j< gridHeight; j += 16. ) {
		gl::drawLine( Vec2f( 0., j ), Vec2f( gridWidth, j ) );	
	}
	gl::drawLine( Vec2f( 0., gridHeight ), Vec2f( gridWidth, gridHeight ) );

	mGridTexture = gridBuffer.getTexture();
	unbindFbo( gridBuffer );
}

void uvToolApp::bindFbo( gl::Fbo buffer )
{
	// bind the FBO, so we can draw on it
	buffer.bindFramebuffer();
	
	// set the correct viewport and matrices
	glPushAttrib( GL_VIEWPORT_BIT );
	gl::setViewport( buffer.getBounds() );
	
	gl::pushMatrices();
	gl::setMatricesWindow( buffer.getSize() );

}

void uvToolApp::unbindFbo( gl::Fbo buffer )
{
	// restore matrices and viewport
	gl::popMatrices();
	glPopAttrib();

	// unbind the FBO to stop drawing on it
	buffer.unbindFramebuffer();
}
CINDER_APP_BASIC( uvToolApp, RendererGl( RendererGl::AA_NONE ) )
