
#include "imageloadernode.h"

#include <QUrl>
#include <QImageReader>

#include "fugio/global_interface.h"
#include "fugio/context_interface.h"
#include <fugio/node_interface.h>
#include <fugio/core/variant_interface.h>
#include <fugio/file/filename_interface.h>

#include <fugio/image/uuid.h>

ImageLoaderNode::ImageLoaderNode( QSharedPointer<fugio::NodeInterface> pNode )
	: NodeControlBase( pNode )
{
	mPinFileName = pinInput( "Filename" );

	mImage = pinOutput<fugio::ImageInterface *>( "Image", mPinImage, PID_IMAGE );
}

ImageLoaderNode::~ImageLoaderNode( void )
{
}

QWidget *ImageLoaderNode::gui( void )
{
	QLabel		*GUI = new QLabel();

	GUI->setFixedSize( 80, 80 );

	connect( this, SIGNAL(pixmapUpdated(QPixmap)), GUI, SLOT(setPixmap(QPixmap)) );

	if( !mImageData.isNull() )
	{
		GUI->setPixmap( QPixmap::fromImage( mImageData.scaledToWidth( 80 ) ) );
	}

	return( GUI );
}

void ImageLoaderNode::inputsUpdated( qint64 pTimeStamp )
{
	QString		ImageFileName;

	NodeControlBase::inputsUpdated( pTimeStamp );

	QUrl		ImageUrl;

	if( mPinFileName->isUpdated( pTimeStamp ) )
	{
		fugio::FilenameInterface		*F = input<fugio::FilenameInterface *>( mPinFileName );

		if( F )
		{
			ImageFileName = F->filename();

			ImageUrl = QUrl::fromLocalFile( ImageFileName );
		}
		else
		{
			ImageFileName = variant( mPinFileName ).toString();
		}
	}

	if( ImageFileName.isEmpty() )
	{
		return;
	}

	if( ImageUrl.isEmpty() )
	{
		ImageUrl = QUrl( ImageFileName );
	}

	if( !ImageUrl.isLocalFile() )
	{
		return;
	}

	QImageReader	ImageReader( ImageUrl.toLocalFile() );

	QImage			IMG = ImageReader.read();

	if( IMG.isNull() )
	{
		qWarning() << ImageUrl.toLocalFile() << ImageReader.errorString();

		//qDebug() << QImageReader::supportedImageFormats();

		return;
	}

	mImageData = IMG;

	mImage->setSize( mImageData.width(), mImageData.height() );

	mImage->setLineSize( 0, mImageData.bytesPerLine() );

	if( mImageData.format() == QImage::Format_RGB32 )
	{
		mImage->setFormat( fugio::ImageInterface::FORMAT_BGRA8 );
	}
	else if( mImageData.format() == QImage::Format_ARGB32 )
	{
		mImage->setFormat( fugio::ImageInterface::FORMAT_BGRA8 );
	}

	memcpy( mImage->internalBuffer( 0 ), mImageData.constBits(), mImageData.byteCount() );

	mNode->context()->pinUpdated( mPinImage );

	emit pixmapUpdated( QPixmap::fromImage( mImageData.scaledToWidth( 80 ) ) );
}
