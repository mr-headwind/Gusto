#!/bin/sh

# Set up AutoTools directory structure.
# Pretty blunt - just deletes whatever is there and starts over

Dev=Development
Rel=Release

curdir=`pwd`

if test -d $HOME/$Rel/Gusto
then
    rm -rf $HOME/$Rel/Gusto
fi

mkdir -p $HOME/$Rel/Gusto
cd $HOME/$Rel/Gusto
mkdir src
mkdir -p data/icons

ln -s $HOME/$Dev/Gusto/README .
ln -s $HOME/$Dev/Gusto/COPYING .
ln -s $HOME/$Dev/Gusto/AutoTools/AUTHORS .
ln -s $HOME/$Dev/Gusto/AutoTools/ChangeLog .
ln -s $HOME/$Dev/Gusto/AutoTools/INSTALL .
ln -s $HOME/$Dev/Gusto/AutoTools/NEWS .
ln -s $HOME/$Dev/Gusto/AutoTools/configure.ac .
ln -s $HOME/$Dev/Gusto/AutoTools/Makefile.am .

cd src
ln -s $HOME/$Dev/Gusto/AutoTools/src/Makefile.am .
cp -p $HOME/$Dev/Gusto/src/*.c .
cp -p $HOME/$Dev/Gusto/src/*.h .

cd ../data
ln -s $HOME/$Dev/Gusto/AutoTools/data/gusto.1 .
ln -s $HOME/$Dev/Gusto/AutoTools/data/gusto.desktop .
ln -s $HOME/$Dev/Gusto/AutoTools/data/Makefile.am .

cd icons
ln -s $HOME/$Dev/Gusto/AutoTools/data/icons/gusto.png .
ln -s $HOME/$Dev/Gusto/AutoTools/data/icons/Makefile.am .

cd ../..

autoreconf -i

cd $curdir

exit 0
