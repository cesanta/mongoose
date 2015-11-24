
#############################################################
# Required variables for each makefile
# Discard this section from all parent makefiles
# Expected variables (with automatic defaults):
#   CSRCS (all "C" files in the dir)
#   SUBDIRS (all subdirs with a Makefile)
#   GEN_LIBS - list of libs to be generated ()
#   GEN_IMAGES - list of images to be generated ()
#   COMPONENTS_xxx - a list of libs/objs in the form
#     subdir/lib to be extracted and rolled up into
#     a generated lib/image xxx.a ()
#
ifndef PDIR
GEN_LIBS = libuser.a
endif


#############################################################
# Configuration i.e. compile options etc.
# Target specific stuff (defines etc.) goes in here!
# Generally values applying to a tree are captured in the
#   makefile at its root level - these are then overridden
#   for a subtree within the makefile rooted therein
#
DEFINES += -DMG_DISABLE_DAV -DMG_DISABLE_SYNC_RESOLVER -DMG_DISABLE_CGI \
           -DMG_DISABLE_SOCKETPAIR -DMG_DISABLE_DIRECTORY_LISTING \
           -DMG_MAX_HTTP_HEADERS=20 -DMG_MAX_HTTP_REQUEST_SIZE=1024 \
           -DMG_MAX_PATH=40 -DMG_MAX_HTTP_SEND_IOBUF=1024 \
           -DMG_ESP8266 -DRTOS_SDK -DMG_LWIP

#############################################################
# Recursion Magic - Don't touch this!!
#
# Each subtree potentially has an include directory
#   corresponding to the common APIs applicable to modules
#   rooted at that subtree. Accordingly, the INCLUDE PATH
#   of a module can only contain the include directories up
#   its parent path, and not its siblings
#
# Required for each makefile to inherit from the parent
#

INCLUDES += -I ./
PDIR := ../$(PDIR)
sinclude $(PDIR)Makefile

