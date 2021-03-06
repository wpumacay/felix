
#include <qtUtils/LQtUtils.h>
#include <cassert>

namespace felix {
namespace qt {

    // Damn you qimage format!!!! xD

    core::LMatf qImage2mat( QImage image )
    {
        assert( !image.isNull() );

        int _nChannels = 3;

        if ( image.isGrayscale() ) 
        { 
            std::cout << "using 1 channel - grayscale!!!" << std::endl;
            _nChannels == 1; 
        }
        else if ( image.hasAlphaChannel() ) 
        { 
            std::cout << "using 4 channels!!!" << std::endl;
            _nChannels == 4; 
        }

        assert( _nChannels == 3 ||
                _nChannels == 1 );

        core::LMatu _matu8( image.height(), 
                            image.width(), 
                            _nChannels );

        u8* _imgBuffer = ( u8* ) image.bits();
        u8* _matBuffer = _matu8.buffer();

        int _w = image.width();
        int _h = image.height();

        for ( int i = 0; i < _h; i++ )
        {
            for ( int j = 0; j < _w; j++ )
            {
                int _pixIndx = j + i * _w;

                _matBuffer[ 3 * _pixIndx + 0 ] = _imgBuffer[ 4 * _pixIndx + 2 ];
                _matBuffer[ 3 * _pixIndx + 1 ] = _imgBuffer[ 4 * _pixIndx + 1 ];
                _matBuffer[ 3 * _pixIndx + 2 ] = _imgBuffer[ 4 * _pixIndx + 0 ];
            }
        }

        return utils::toMatf( _matu8 );
    }

    QImage mat2qImage( const core::LMatf& mat )
    {
        auto _format = QImage::Format_RGB32;

        if ( mat.channels() == 1 )
        {
            std::cout << "using 1 channel - grayscale!!!" << std::endl;
            _format = QImage::Format_Indexed8;
        }
        else if ( mat.channels() == 4 )
        {
            std::cout << "using 4 channels!!!" << std::endl;
            _format = QImage::Format_ARGB32;
        }

        assert( mat.channels() == 3 ||
                mat.channels() == 1 );

        core::LMatu _matu8 = utils::toMatu( mat );

        u8* _imgBuffer = NULL;
        QImage _qimage;

        if ( mat.channels() == 3 )
        {
            _imgBuffer = new u8[ 4 * _matu8.cols() * _matu8.rows() ];
            u8* _matBuffer = _matu8.buffer();

            int _w = _matu8.cols();
            int _h = _matu8.rows();

            for ( int i = 0; i < _h; i++ )
            {
                for ( int j = 0; j < _w; j++ )
                {
                    int _pixIndx = j + i * _w;

                    _imgBuffer[ 4 * _pixIndx + 0 ] = _matBuffer[ 3 * _pixIndx + 2 ];
                    _imgBuffer[ 4 * _pixIndx + 1 ] = _matBuffer[ 3 * _pixIndx + 1 ];
                    _imgBuffer[ 4 * _pixIndx + 2 ] = _matBuffer[ 3 * _pixIndx + 0 ];
                }
            }

            _qimage = QImage( _imgBuffer, _matu8.cols(), _matu8.rows(), _format );
        }
        else if ( mat.channels() == 1 )
        {
            _imgBuffer = new u8[ _matu8.cols() * _matu8.rows() ];
            u8* _matBuffer = _matu8.buffer();

            int _w = _matu8.cols();
            int _h = _matu8.rows();

            for ( int i = 0; i < _h; i++ )
            {
                for ( int j = 0; j < _w; j++ )
                {
                    int _pixIndx = j + i * _w;

                    _imgBuffer[ _pixIndx ] = _matBuffer[ _pixIndx ];
                }
            }

            QVector<QRgb> _colorTable( 256 );
            for ( int i = 0; i < 256; i++ )
            {
                _colorTable[i] = qRgb( i, i, i ); //build palette
            }

            _qimage = QImage( _imgBuffer, _matu8.cols(), _matu8.rows(), _format );
            _qimage.setColorCount( 256 );
            _qimage.setColorTable( _colorTable );
        }


        return _qimage;
    }

}}