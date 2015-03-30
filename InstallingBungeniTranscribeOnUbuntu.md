# Introduction #

Bungeni Transcribe requires libVLC 1.1.0 or later and libqxt 0.6.1 or later.

# Source #

Install qt4 and mercurial if it they are not already installed.

```
sudo apt-get install libqt4-dev mercurial
hg clone https://code.google.com/p/bungeni-transcriber/
cd bungeni-transcriber
```

# libVLC #
## Ubuntu 10.10 and later ##

Supported versions of libVLC are available in the repositories of Ubuntu 10.10 and later
```
sudo apt-get install libvlc-dev
sudo apt-get build-dep vlc
```

## Ubuntu 10.04 and earlier ##
```
wget http://download.videolan.org/pub/videolan/vlc/1.1.13/vlc-1.1.13.tar.bz2
tar -xvf vlc-1.1.13.tar.bz2
cd vlc-1.1.13
./configure --prefix=$SOMEPATH
make
make install
sudo ldconfig $SOMEPATH
```
Modify INCLUDEPATH in transcribe.pro to point to $SOMEPATH

# libqxt #

## Ubuntu 11.10 and later ##

```
sudo apt-get install libqxt-dev
```

## Ubuntu 11.04 and earlier ##

```
wget http://dev.libqxt.org/libqxt/get/v0.6.2.zip
unzip v0.6.2.zip
cd 
./configure -prefix=$SOMEPATH
make
make install
sudo ldconfig $SOMEPATH
```
Modify transcribe.pro and add $SOMEPATH to INCLUDEPATH

# Build and run #
```
qmake
make
./transcribe
```