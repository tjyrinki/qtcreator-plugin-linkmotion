# Link Motion QtCreator Plugins

This repository contains developer plugins for the Qt Creator.

There are multiple plugin files in order not to end up into one huge plugin which does everything. So the core idea that one plugin should provide one feature (or do one thing).

Then when all these plugins are installed you will have the freedom to deselect or select which plugins you are using with the Qt Creator.

The exception is the *share* folder which is copied as-is to the Qt Creator path, so if you need to include scripts or external files which are not inside a qrc file then this is the location in this repository.



# Description of the plugins
## Welcome
This plugin provides a welcome screen, which should at the end contain links to developer documentations and guides. It should also contain any other valid information and contact methods for further support.

See *src/lmwelcomeplugin* for code.

# Building the plugins
**NOTE!** When you are building the Qt Creator you **MUST** use the exact version of the Qt which was used to build the Qt Creator. For example in Ubuntu use the qmake found in /usr/bin/qmake. You might have something else in your path.

## Locally for installation to HOME folder.
### Setup and preparations
You can check the current build dependencies from *debian/control* file, but at the time of writing of this markdown document following packages were enough:
```
sudo apt-get install qtcreator-dev qt5-default qt5declarative-dev
```

### Building the binaries
You can build the plugin instance to your home folder with
```
 qmake -r CONFIG+=BUILDTOHOME 
 make
```

## Using Cowbuilder for building a package
You can either use a helper script which is available *package-create*, which will create deb packages for you under *pkg* folder, or continue reading.

### Setup and preparations
**NOTE!** The cowbuilder does not like symbolic links or partitions much. So if you get into issues then you might need to adjust the *~/.pbuilderrc* file so that everything happens inside one partition.

Install the required packages:
```
 sudo apt-get install ubuntu-dev-tools cowbuilder
```
Now you have the developer tools installed for building packages with the cowbuilder. The next step is to prepare a build environment:
```
 cowbuilder-dist xenial amd64 create
```
That command created an Ubuntu build root for the xenial distribution. You can change the value from xenial to any other debian/ubuntu distribution to which you would like to target your package.

### Build a package
```
 debuild -S -uc -us
 cowbuilder-dist xenial amd64 build ../qtcreator-plugin-linkmotion_3.5.0+16.04.20160524-0ubuntu2.dsc
```

## Running the plugin
### Local build to HOME
You should just start the qtcreator which is in the path. For example open terminal and type 'qtcreator' and press *enter*.

# Questions?
## The code does not compile, does not find -lCore (etc)
You have compiled your code against a different version of Qt than what the Qt Creator itself has been compiled at.

**Please check** that the *qmake* which you used is the right one.

## Something else
Drop an email or jabber message to:  
Juhapekka Piiroinen <juhapekka.piiroinen@link-motion.com>
