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
#include "cinder/gl/texture.h"
#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"
#include "boost/algorithm/string.hpp" 

#include "cinder/params/Params.h"
#include "cinder/ip/Fill.h"
#include "cinder/ip/Grayscale.h"

#include "CinderOpenCv.h"

using namespace ci;
using namespace ci::app;
using namespace std;

enum CONSTRUCTION_STATES {
	STATE_LOAD,
	STATE_EXTREMES,
	STATE_GRAYCODE,
	STATE_BINARY,
	STATE_COMBINE,
	STATE_CLEANUV,
	STATE_INVERSE,
	STATE_CLEANINVERSE,
	STATE_FILLINVERSE,
	STATE_IDLE
};

enum DO_PASSES {
	DO_ALL = 0,
	DO_UV,
	DO_INVERSE,
	DO_FILL
};

class uvConstructionApp : public AppBasic {
public:
	void setup();
	void update();
	void keyDown( KeyEvent event );
	void draw();

private:
	void browse();
	void startProcessing();
	void showMessage( string aMessage, bool isError );

	// processing passes
	void loadPass();
	void extremesPass();
	void graycodePass();
	void binaryPass();
	void combinePass();
	void cleanUVPass();
	void inversePass();
	void cleanInversePass();
	void fillInversePass();

	// utility methods
	string   argument( string argumentName, string defaultValue );
	
	void     removeSpeckles( Surface16u map, int windowSize );
	uint16_t median16u(vector<uint16_t> data);

	// settings and arguments
	params::InterfaceGl	mParams;

	bool mInteractive;
	int mPasses;
	int mChannel;
	// The following parameters must be int32_t for params::addParam
	int32_t mBits;					
	int32_t mProjectionWidth;
	int32_t mProjectionHeight;
	int32_t mInverseThreshold;
	int32_t mMinFillRadius;
	int32_t mMaxFillRadius;

	string mFirstFilePath;
	string mUVFileName;
	string mInverseFileName;
	string mProjectionFileName;

	int32_t mVSkip;
	int32_t mAsymmetricPatterns;	
	int32_t mPatternContrast;
	int32_t mDespeckleRadius;
	bool mCentered;
	bool mInverseFrame;

	// created channels and surfaces
	vector<Channel>	mPattern;
	Channel		mDarkChannel;
	Channel		mLightChannel;
	Channel		mAlphaChannel;
	Surface16u	mUVMap;
	Surface16u	mInverseMap;

	// runtime variables
	string mFilePathReplace;
	int mMedianWindow;
	int mFirstNumU;
	int mFirstNumV;

	gl::Texture	mProgressTexture;

	int mCounter;
	int mState;
	int mAxis;
};

void uvConstructionApp::setup()
{
	// get parameters from commandline

	try {
		mFirstFilePath = argument( "pattern","" );
		mVSkip = fromString<int>( argument( "vskip", "0" ) );
	
		mUVFileName = argument( "uvmap","uv-map.tif" );
		mInverseFileName = argument( "inversemap","inverse-map.tif" );
		mProjectionFileName = argument( "projectionmap","projection-map.tif" );

		mPasses = max( min( fromString<int>( argument( "pass", "0" ) ), (int)DO_FILL), (int)DO_ALL);
		mBits = max( min( fromString<int>( argument( "bits", "10" ) ), 16), 4);
		mChannel = max( min( fromString<int>( argument( "channel", "0" ) ), 3), 0);
		mProjectionWidth = max( min( fromString<int>( argument( "width", "1024" ) ), 2 << ( mBits - 1 ) ), 1);
		mProjectionHeight = max( min( fromString<int>( argument( "height", "768" ) ), 2 << ( mBits - 1 ) ), 1);
		mCentered = fromString<bool>( argument( "centered", "1" ) );
		mInverseFrame = fromString<bool>( argument( "inverseframe", "1" ) );

		mAsymmetricPatterns = max( min( fromString<int>( argument( "asymmetricpatterns", "2" ) ), (int)mBits - 1 ), 0);
		mPatternContrast = max( fromString<int>( argument( "contrast", "8" ) ), 1);
		mDespeckleRadius = max( min( fromString<int>( argument( "despeckleradius", "1" ) ), 10), 0);

		mInverseThreshold = 256 * max( min( fromString<int>( argument( "inversethreshold", "0" ) ), 255), 0);
		mMinFillRadius = max( min( fromString<int>( argument( "minfillradius", "0" ) ), 10), 0);
		mMaxFillRadius = max( min( fromString<int>( argument( "maxfillradius", "5" ) ), 10), 0);

		if (mFirstFilePath == "" )
			mInteractive = true;
		else
			mInteractive = fromString<bool>( argument( "interactive", "0" ) );	
	}
	catch( ... ) {
		showMessage( "Unable to parse commandline options.", true );
		return;
	};
	
	if ( !mInteractive ) {
		startProcessing();
	} else {
		// setup parameter interface
		
		mParams = params::InterfaceGl( "UV construction", Vec2i( 200, 400 )  );
		mParams.setOptions ( "", "size=`340 400` valueswidth=160 fontresizable=false help=`This application creates a projection-map from a number of photographed graycode callibration patterns.`" );

		vector<string> passNames;
		passNames.push_back( "All steps" ); passNames.push_back( "Create uv-map" ); passNames.push_back( "Create inverse-map" ); passNames.push_back( "Fill gaps" );  
		mParams.addParam( "Pass(es)", passNames, &mPasses ,"" );
		mParams.addButton( "Start processing", std::bind( &uvConstructionApp::startProcessing, this ) );
		mParams.addText( "messages", "label=` `" );
		mParams.addSeparator();

		mParams.addParam( "First pattern or map", &mFirstFilePath, "group=`Common settings`", true );
		mParams.addButton( "Browse", std::bind( &uvConstructionApp::browse, this ), "group=`Common settings`" );
		mParams.addParam( "Bits", &mBits, "group=`Common settings` min=4 max=15" );
		mParams.addSeparator();
		
		mParams.addParam( "Filename1", &mUVFileName, "group=`1. uv-map` label=Filename" );
		vector<string> channelNames;
		channelNames.push_back( "Red" ); channelNames.push_back( "Green" ); channelNames.push_back( "Blue" ); channelNames.push_back( "Grayscale" ); 
		mParams.addParam( "Color channel", channelNames, &mChannel ,"group=`1. uv-map`" );
		mParams.addParam( "Inverse frame", &mInverseFrame, "group=`1. uv-map`" );
		mParams.addParam( "Asymmetric patterns", &mAsymmetricPatterns, "group=`1. uv-map` min=0 max=15" );
		mParams.addParam( "Contrast", &mPatternContrast, "group=`1. uv-map` min=1 max=127" );
		mParams.addParam( "Despeckle1", &mDespeckleRadius, "group=`1. uv-map` label=`Despeckle radius` min=0 max=10" );
		mParams.addParam( "Skip between h.& v.", &mVSkip, "group=`1. uv-map`" );
		//mParams.setOptions( "1. uv-map", "opened=false" );

		mParams.addParam( "Filename2", &mInverseFileName, "group=`2. Inverse-map` label=Filename" );
		mParams.addParam( "Width", &mProjectionWidth, "group=`2. Inverse-map` min=64 max=4096 step=8" );
		mParams.addParam( "Height", &mProjectionHeight, "group=`2. Inverse-map` min=64 max=4096 step=8" );
		mParams.addParam( "Centered patterns", &mCentered, "group=`2. Inverse-map`" );
		mParams.addParam( "Threshold", &mInverseThreshold, "group=`2. Inverse-map` min=0 max=255" );
		mParams.addParam( "Despeckle2", &mDespeckleRadius, "group=`2. Inverse-map` label=`Despeckle radius` min=0 max=10" );
		//mParams.setOptions( "2. Inverse-map", "opened=false" );
		
		mParams.addParam( "Filename3", &mProjectionFileName, "group=`3. Fill gaps` label=Filename" );
		mParams.addParam( "Minimum radius", &mMinFillRadius, "group=`3. Fill gaps` min=0 max=10" );
		mParams.addParam( "Maximum radius", &mMaxFillRadius, "group=`3. Fill gaps` min=0 max=10" );
		//mParams.setOptions( "3. Fill gaps", "opened=false" );

		mState = STATE_IDLE;
	}
}

void uvConstructionApp::update()
{
	switch (mState) {
		case STATE_LOAD:
			loadPass();
			break;
			
		case STATE_EXTREMES:
			extremesPass();
			break;
			
		case STATE_GRAYCODE:
			graycodePass();
			break;
			
		case STATE_BINARY:
			binaryPass();
			break;
			
		case STATE_COMBINE:
			combinePass();
			break;
			
		case STATE_CLEANUV:
			cleanUVPass();
			break;
			
		case STATE_INVERSE:
			inversePass();
			break;

		case STATE_CLEANINVERSE:
			cleanInversePass();
			break;

		case STATE_FILLINVERSE:
			fillInversePass();
			break;

		default:
			if ( !mInteractive ) 
				quit();
			break;
	}
}

void uvConstructionApp::keyDown( KeyEvent event )
{
	string path;

	switch ( event.getCode() ) {
		case KeyEvent::KEY_ESCAPE:
			if ( mState != STATE_IDLE ) {
				mState = STATE_IDLE;
				showMessage( "Pass aborted by the user.", false );
			}

			break;
	}
}


void uvConstructionApp::draw()
{
	if (!mProgressTexture) 
		gl::clear( Color( 0, 0, 0) );
	else
		gl::draw( mProgressTexture, getWindowBounds() );

	if ( mInteractive )
		mParams.draw();
}

void uvConstructionApp::browse()
{
	string fileName = getOpenFilePath();
	if ( !fileName.empty() )
		mFirstFilePath = fileName;
}

void uvConstructionApp::startProcessing()
{
	showMessage( "", false );

	mCounter = 0;
	mMedianWindow = ( mDespeckleRadius != 0 ) ? 1 + 2 * mDespeckleRadius : 0;

	switch(mPasses) {
		case DO_FILL:
			// start by loading the reconstructed mInverseMap, and run from there
			try {
				mInverseMap = Surface16u( loadImage( getPathDirectory( mFirstFilePath ) + mInverseFileName ) );
			}
			catch( ... ) {
				showMessage( "Unable to load the inverse-map. Run the first two passes first!", true );
				return;
			};
			
			mProgressTexture = gl::Texture( mInverseMap );

			mCounter = mMinFillRadius-1;
			mState = STATE_FILLINVERSE;
			break;

		case DO_INVERSE:
			// start by loading the reconstructed mUVMap, and run from there
			try {
				mUVMap = Surface16u( loadImage( getPathDirectory( mFirstFilePath ) + mUVFileName ) );
			}
			catch( ... ) {
				showMessage( "Unable to load the uv-map. Run the first pass first!", true );
				return;
			};

			mProgressTexture = gl::Texture( mUVMap );

			mState = STATE_INVERSE;
			break;

		case DO_UV:
		default:
			// start at the beginning, loading the raw pattern files

			if (mFirstFilePath == "" ) {
				showMessage( "No pattern path provided!", true );
				return;
			}

			// get the last group of digits as the basis the sequence of filenames
			string fileName = getPathFileName(mFirstFilePath);
			size_t lastDigit = fileName.find_last_of( "0123456789" );
			if (lastDigit == -1) {
				showMessage( "Could not determine series of files from the provided filename.", true );
				return;
			}
			size_t firstDigit = fileName.find_last_not_of( "0123456789", lastDigit);
			mFilePathReplace = fileName.substr(firstDigit+1, lastDigit-firstDigit);

			mFirstNumU = fromString<int>(mFilePathReplace);
			mFirstNumV = mFirstNumU + mBits + ( (mInverseFrame) ? 1 : 0 ) + mVSkip;

			// resize the pattern vector to contains all patterns
			mPattern.resize(mBits + ( (mInverseFrame) ? 1 : 0 ));

			mAxis = 0;

			mState = STATE_LOAD;
			break;
	};
}

void uvConstructionApp::showMessage( string aMessage, bool isError )
{
	if ( mInteractive ) {
		string optionsString = "label=`" + aMessage + " `";
		mParams.setOptions( "messages", optionsString );
	} else {
		console() << ( ( isError ) ? "ERROR: " : "" ) << aMessage << endl;

		if ( isError )
			quit();
	}
}


string uvConstructionApp::argument( string aArgumentName, string aDefaultValue = "" )
{
	// utility method: find argument from commandline arguments
	for ( vector<string>::const_iterator argIter = getArgs().begin(); argIter != getArgs().end(); ++argIter ) {
		if ( ( "-"+aArgumentName) == *argIter && argIter != getArgs().end() ) {
			++argIter;
			return *argIter;
		}
	}
	return aDefaultValue;
}


void uvConstructionApp::removeSpeckles( Surface16u aMap, int aWindowSize )
{
	// Remove stray pixels by calculating a median-blurred version of the image 
	// and looking at the difference between the median and the original image

	if ( aWindowSize == 0 )
		return;

	cv::Mat mapOcv( toOcv( aMap ) );
	cv::Mat medianOcv, diffOcv;

	cv::medianBlur( mapOcv, medianOcv, aWindowSize );
	cv::absdiff( mapOcv, medianOcv, diffOcv );

	Surface diffMap = Surface( fromOcv( diffOcv ) );

	Surface16u::Iter mapIter( aMap.getIter() );
	Surface::Iter diffMapIter( diffMap.getIter() );
	while ( mapIter.line() ) {
		diffMapIter.line();
		while ( mapIter.pixel() ) {
			diffMapIter.pixel();

			if ( ( diffMapIter.r() > 0 ) || ( diffMapIter.g() > 0 ) ) {
				mapIter.a() = 0;
				mapIter.r() = 0;
				mapIter.g() = 0;
			}
		}
	}
}

uint16_t uvConstructionApp::median16u(vector<uint16_t> aData)
{
    typedef vector<uint16_t>::size_type vec_sz;

	uint16_t median;
    vec_sz size = aData.size();
    vec_sz mid = size/2;
	
	nth_element( aData.begin(), aData.begin()+mid, aData.end() );

	median = aData[mid];
	if ( size % 2 == 0 ) 
		median += (int)( aData[mid-1] - median ) / 2; 

    return median;
}


void uvConstructionApp::loadPass() 
{
	// load image and extract a channel
	string pattern_nr = toString(mCounter + ( (mAxis==0)?mFirstNumU:mFirstNumV) );

	// modify the path to the first image, taking into account leading 0s
	int numberLength = max( pattern_nr.length(), mFilePathReplace.length() );
	string fileNumber = ( "00000000" + pattern_nr ).substr( pattern_nr.length() + 8 - numberLength, numberLength );
	string filePath = getPathDirectory( mFirstFilePath ) + boost::replace_last_copy( getPathFileName( mFirstFilePath ), mFilePathReplace, fileNumber ); 

	// try to load the image
	try {
		Surface imported( loadImage( filePath ) );
		if ( mChannel != 3 )
			mPattern[ mCounter ] = Channel ( imported.getChannel( mChannel ) ).clone();
		else {
			Channel gray = Channel( imported.getWidth(), imported.getHeight() );
			ip::grayscale( imported, &gray );
			mPattern[ mCounter ] =  gray;
		}
	}
	catch( ... ) {
		showMessage( "Unable to load pattern file " + filePath, true );
		return;
	};

	mProgressTexture = gl::Texture( mPattern[ mCounter ] );

	mCounter++;
	if ( mCounter >= mBits + ( ( mInverseFrame ) ? 1 : 0 ) ) {
		mDarkChannel = mPattern[ mAsymmetricPatterns ].clone();
		mLightChannel = mPattern[ mAsymmetricPatterns ].clone();
		mAlphaChannel = mDarkChannel.clone( false );	
		
		if (mAxis==0) {
			mUVMap = Surface16u( mAlphaChannel.getWidth(), mAlphaChannel.getHeight(), true, SurfaceChannelOrder::RGBA );
			ip::fill( &mUVMap, ColorA( 0, 0, 0, 0 ) );
		}
		
		mCounter = 0;
		mState++;
	}	
}

void uvConstructionApp::extremesPass() 
{
	int i;
	if (mCounter < ( mBits - ( ( mInverseFrame ) ? 0 : 1 ) - mAsymmetricPatterns ) ) {
		// extract lightest & darkest from "symmetric" patterns first
		i = mCounter + mAsymmetricPatterns + 1;
	} else {
		// then clamp "asymmetric" patterns
		i = mCounter + mAsymmetricPatterns - mBits + ( ( mInverseFrame ) ? 0 : 1 );
	}
	Channel::Iter darkIter( mDarkChannel.getIter() );
	Channel::Iter lightIter( mLightChannel.getIter() );
	Channel::Iter patternIter( mPattern[ i ].getIter() );
		
	while ( patternIter.line() ) {
		darkIter.line();
		lightIter.line();
		while ( patternIter.pixel() ) {
			darkIter.pixel();
			lightIter.pixel();
				
			if (mCounter < ( mBits - mAsymmetricPatterns ) ) {
				// get lightest and darkest pixels, skipping the first (asymmetric) patterns
				if ( patternIter.v() > lightIter.v() ) {
					lightIter.v() = patternIter.v();
				}
				if ( patternIter.v() < darkIter.v() ) {
					darkIter.v() = patternIter.v();
				}
			} else {
				// clamp skipped (asymmetric) patterns to the light & dark patterns
				if (patternIter.v() > lightIter.v() ) {
					patternIter.v() = lightIter.v();
				}
				if (patternIter.v() < darkIter.v() ) {
					patternIter.v() = darkIter.v();
				}
			}
		}				
	}

	mProgressTexture = gl::Texture( mDarkChannel );
	
	mCounter++;
	if ( mCounter > mBits - ( ( mInverseFrame ) ? 0 : 1 ) ) {
		// discard inverseframe
		if ( mInverseFrame )
			mPattern[mBits].reset();

		// construct alpha from lightest and darkest pixels
		Channel::Iter darkIter( mDarkChannel.getIter() );
		Channel::Iter lightIter( mLightChannel.getIter() );
		Channel::Iter alphaIter( mAlphaChannel.getIter() );
		Channel16u::Iter uvAlphaIter( mUVMap.getChannelAlpha().getIter() );
		while ( alphaIter.line() ) {
			darkIter.line();
			lightIter.line();
			uvAlphaIter.line();
			while ( alphaIter.pixel() ) {
				darkIter.pixel();
				lightIter.pixel();
				uvAlphaIter.pixel();
				
				alphaIter.v() = lightIter.v() - darkIter.v();
				
				// warning, optimisation trick ahead!
				/* I need to store both (8 bit) Light and Dark extremes, and I have 
				   only one (16 bit) Alpha channel to store it in... Oh, whatever shall I do? */
				if (mAxis == 0) {
					uvAlphaIter.v() = lightIter.v() + 256 * darkIter.v();
				} else {
					// retreive Light and Dark values from previous axis
					uint8_t lightest = (uint16_t)uvAlphaIter.v() & (uint16_t)255;
					uint8_t darkest = (uint16_t)uvAlphaIter.v() >> 8;
					
					// create 16 bit alpha from darkest Dark and lightest Light
					uvAlphaIter.v() = 256 *
						( (max( (uint8_t)lightIter.v(), lightest) ) -
						  (min( (uint8_t)darkIter.v(), darkest) ) );
				}
			}
		}
		
		mCounter = 0;
		mState++;
	}
}

void uvConstructionApp::graycodePass()
{
	// remove black level from patterns and amplify them by alpha
	Channel::Iter darkIter( mDarkChannel.getIter() );
	Channel::Iter alphaIter( mAlphaChannel.getIter() );
	Channel::Iter patternIter( mPattern[ mCounter ].getIter() );
	
	while ( patternIter.line() ) {
		darkIter.line();
		alphaIter.line();
		while ( patternIter.pixel() ) {
			darkIter.pixel();
			alphaIter.pixel();
			
			if ( alphaIter.v() > 0 ) {
				patternIter.v() = (int)min( 255, max( 0,
									( 255 * (int)mPatternContrast * (int)(patternIter.v()-darkIter.v() ) / alphaIter.v() ) - (int)( 127 * mPatternContrast ) ) );
			} else {
				patternIter.v() = 0;
			}
		}
	}

	mProgressTexture = gl::Texture( mPattern[ mCounter ] );
	mCounter++;
	
	if ( mCounter >= mBits ) {
		mCounter = 1;
		mState++;
	}
}

void uvConstructionApp::binaryPass()
{
	// XOR pattern with previous pattern to convert from graycode to regular binary
	Channel::Iter patternIter( mPattern[ mCounter ].getIter() );
	Channel::Iter previousPatternIter( mPattern[ mCounter-1 ].getIter() );
	
	while ( patternIter.line() ) {
		previousPatternIter.line();
		while ( patternIter.pixel() ) {
			previousPatternIter.pixel();
			
			patternIter.v() = patternIter.v() ^ previousPatternIter.v();
		}
	}

	mProgressTexture = gl::Texture( mPattern[ mCounter ] );
	mCounter++;
	
	if (mCounter >= mBits) {
		mCounter = 0;
		mState++;
	}
}

void uvConstructionApp::combinePass()
{
	// add patterns to result channel
	Channel::Iter patternIter( mPattern[ mCounter ].getIter() );
	Channel16u::Iter mapIter( mUVMap.getChannel( mAxis ).getIter() );
	
	while ( patternIter.line() ) {
		mapIter.line();
		while ( patternIter.pixel() ) {
			mapIter.pixel();
			
			mapIter.v() = mapIter.v() + (uint16_t)(pow( (double)2,15-(int)mCounter) * patternIter.v() / (255) );
		}
	}

	mProgressTexture = gl::Texture( mUVMap );

	mCounter++;
	
	if ( mCounter >= mBits ) {
		mCounter = 0;
		if ( mAxis==0 ) {
			mAxis++;
			mState = 0;
		} else {
			// dispose of the patterns, we don't need them anymore
			for ( int i = 0; i < mBits; i++ ) {
				mPattern[i].reset();
			}

			mState++;
		}
	}
}

void uvConstructionApp::cleanUVPass()
{
	// remove stray pixels from uv-map

	removeSpeckles( mUVMap, mMedianWindow );

	mProgressTexture = gl::Texture( mUVMap );

	writeImage( getPathDirectory( mFirstFilePath ) + mUVFileName, mUVMap );
	if ( mPasses != DO_UV ) {
		mState++;
	} else {
		mState = STATE_IDLE;
	}
}


void uvConstructionApp::inversePass()
{
	// invert created uv-map
	mInverseMap = Surface16u( mProjectionWidth,mProjectionHeight, true, SurfaceChannelOrder::RGBA ); 
	ip::fill( &mInverseMap, ColorAT<uint16_t>( 0, 0, 0, 0 ) );
	Channel16u inverseMapAlpha = mInverseMap.getChannelAlpha();

	Surface16u::Iter uvMapIter( mUVMap.getIter() );
	
	uint32_t mapWidth = mUVMap.getWidth() - 1;
	uint32_t mapHeight = mUVMap.getHeight() - 1;
	
	int32_t patternWidth = (int32_t)pow( 2., (double)mBits );

	double coordMultiplier = (double)patternWidth / 65536;
	int xOffset = (mCentered) ? ( ( patternWidth - mProjectionWidth) / 2 ) : 0;
	int yOffset = (mCentered) ? ( ( patternWidth - mProjectionHeight) / 2 )  : 0;

	while ( uvMapIter.line() ) {
		while ( uvMapIter.pixel() ) {
			if ( uvMapIter.a() > mInverseThreshold && uvMapIter.r() > 0 && uvMapIter.g() > 0 ) {
				// multiply u&v to cover 16 bit resolution
				Vec2i mappedCoordinate = Vec2i( (int)( coordMultiplier * uvMapIter.r() - xOffset ), (int)( coordMultiplier * uvMapIter.g() - yOffset ) );
				
				// check if a "better defined" pixel in the uvmap has already "claimed" this mapped coordinate
				uint16_t alphaValue = uvMapIter.a();
				if ( inverseMapAlpha.getValue( mappedCoordinate ) < alphaValue ) {
					mInverseMap.setPixel( mappedCoordinate, ColorT<uint16_t>(
											(uint16_t)( ( (uint32_t)uvMapIter.x() << 16) / mapWidth ), 
											(uint16_t)( ( (uint32_t)uvMapIter.y() << 16) / mapHeight ), 0 ) );
					inverseMapAlpha.setValue( mappedCoordinate, alphaValue );
				}
			}
		}	
	}
	
	// dispose of mUVMap, we don't need it anymore
	mUVMap.reset();

	mProgressTexture = gl::Texture( mInverseMap );
	
	mState++;
}

void uvConstructionApp::cleanInversePass()
{
	// remove stray pixels from uv-map

	removeSpeckles( mInverseMap, mMedianWindow );
	
	mProgressTexture = gl::Texture( mInverseMap );

	writeImage( getPathDirectory( mFirstFilePath ) + mInverseFileName, mInverseMap );
	if ( mPasses != DO_INVERSE ) {
		mState++;
	} else {
		mState = STATE_IDLE;
	}	
}
void uvConstructionApp::fillInversePass() 
{
	// fill holes in the inverse uvmap with a recursive median fill
	
	Surface16u sourceMap = mInverseMap.clone();

	Surface16u::Iter inverseMapIter( mInverseMap.getIter() );
	Surface16u::Iter sourceMapIter( sourceMap.getIter() );

	unsigned int filledPixels = 0;
	unsigned int blackPixels = 0;

	int fillRadius = mCounter + 1;
	unsigned int requiredSamples = 1 + ( fillRadius*fillRadius );

	while ( inverseMapIter.line() ) {
		sourceMapIter.line();

		while ( inverseMapIter.pixel() ) {
			sourceMapIter.pixel();

			if ( sourceMapIter.a() == 0 ) {
				// "black" pixel; see if we can calculate a median from its neighbours
				blackPixels++;

				vector<uint16_t> dataU;
				vector<uint16_t> dataV;

				// sample window for "non-black" pixels
				for ( int i = -fillRadius; i <= fillRadius; i++ ) {
					for ( int j = -fillRadius; j <= fillRadius; j++ ) {
						if ( !(i==0 && j==0) && sqrt( (float)( i * i + j * j ) ) <= fillRadius && sourceMapIter.aClamped( i, j ) > 0 ) {
							dataU.push_back( sourceMapIter.rClamped ( i, j ) );
							dataV.push_back( sourceMapIter.gClamped ( i, j ) );
						}
					}
				}

				if ( dataU.size() > requiredSamples ) {
					inverseMapIter.r() = median16u( dataU );
					inverseMapIter.g() = median16u( dataV );
					inverseMapIter.a() = 65535;
					filledPixels++;
				}
			}
		}
	}
	mProgressTexture = gl::Texture( mInverseMap );

	if ( filledPixels == 0 ) {
		mCounter++;
		if ( blackPixels == 0 || mCounter >= mMaxFillRadius ) {
			// write image, no alpha
			writeImage( getPathDirectory( mFirstFilePath ) + mProjectionFileName, Surface16u( mInverseMap, SurfaceConstraintsDefault(), false ) );

			if ( mPasses != DO_FILL ) {
				mCounter = mMinFillRadius-1;
				mState++;
			} else {
				mState = STATE_IDLE;
			}	
		}
	}
}

CINDER_APP_BASIC( uvPlayerApp, RendererGl(RendererGl::AA_NONE) );
