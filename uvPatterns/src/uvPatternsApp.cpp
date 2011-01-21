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
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class uvPatternsApp : public AppBasic {
  public:
	void setup();
	void keyDown( KeyEvent event );
	void resize( ResizeEvent event );
	void update();
	void draw();

  private:
	Channel makeGraycodes( int aBits );

	Channel mGraycode;
	gl::Texture mTexture;

	int mLine;
	bool mVAxis;

	bool mCentered;
};

void uvPatternsApp::setup()
{
	mCentered = true;
}

void uvPatternsApp::keyDown( KeyEvent event )
{
	switch ( event.getCode() ) {
	case KeyEvent::KEY_ESCAPE:
		if(isFullScreen())
			setFullScreen( false );
		else
			quit();
			
		break;
		
	case KeyEvent::KEY_f:
		setFullScreen( ! isFullScreen() );
			
		break;

	case KeyEvent::KEY_LEFT:
		if ( mLine > 0 )
			mLine--;
		else {
			mLine = mGraycode.getHeight() - 1;
			mVAxis = !mVAxis;
		}

		break;

	case KeyEvent::KEY_RIGHT:
		if ( mLine < ( mGraycode.getHeight() - 1 ) ) 
			mLine++;
		else {
			mLine = 0;
			mVAxis = !mVAxis;
		}

		break;
	}
}

void uvPatternsApp::resize( ResizeEvent event )
{
	int size = max( getWindowWidth(), getWindowHeight() );
	int bits = (int)ceil( log( (double)size ) / log( (double)2 ) );
	mGraycode = makeGraycodes( bits );

	mLine = 0;
	mVAxis = false;
}

void uvPatternsApp::update()
{
	mTexture = gl::Texture( mGraycode );
	mTexture.setMagFilter( GL_NEAREST );
}

void uvPatternsApp::draw()
{
	int32_t patternWidth = mGraycode.getWidth();

	int32_t offset = 0;

	if(mTexture) {
		if( ! mVAxis ) {
			if( mCentered )
				offset = ( patternWidth - getWindowWidth() ) / 2;

			gl::translate( Vec2f( (float)-offset, 0 ) );
			
			gl::draw( mTexture, Area( 0, mLine, mGraycode.getWidth(), mLine+1 ), Area( 0, 0, patternWidth, patternWidth ) ); 
			
			gl::translate( Vec2f( (float)offset, 0 ) );
		} else {
			if( mCentered )
				offset = ( patternWidth - getWindowHeight() ) / 2;

			gl::translate( Vec2f( (float)patternWidth , (float)-offset ) );
			gl::rotate( 90.0 );

			gl::draw( mTexture, Area( 0, mLine, mGraycode.getWidth(), mLine+1 ), Area( 0, 0, patternWidth, patternWidth ) ); 

			gl::rotate( -90.0 );
			gl::translate( Vec2f( -(float)patternWidth , (float)offset ) );
		}
	} else {
		gl::clear( Color( 0, 0, 0 ) ); 
	}
}

Channel uvPatternsApp::makeGraycodes( int aBits ) 
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

CINDER_APP_BASIC( uvPatternsApp, RendererGl )