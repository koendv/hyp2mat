# This file is part of MXE.
# See index.html for further information.

PKG             := hyp2mat
$(PKG)_IGNORE   :=
$(PKG)_VERSION  := 0.0.14
$(PKG)_CHECKSUM := f7603faa905dbb24a639de3cdc160346e54c5714
$(PKG)_SUBDIR   := $(PKG)-$($(PKG)_VERSION)
$(PKG)_FILE     := $(PKG)-$($(PKG)_VERSION).tar.gz
$(PKG)_URL      := https://github.com/koendv/hyp2mat/archive/v$($(PKG)_VERSION).tar.gz
$(PKG)_DEPS     := gcc gtk2

define $(PKG)_UPDATE
    $(WGET) -q -O- 'https://github.com/koendv/hyp2mat/commits/stable' | \
    $(SED) -n 's#.*<span class="sha">\([^<]\{7\}\)[^<]\{3\}<.*#\1#p' | \
    head -1
endef

define $(PKG)_BUILD
    cd '$(1)' && ./bootstrap.sh && ./configure \
        --host='$(TARGET)' \
        --disable-shared \
        --disable-library \
        --prefix='$(PREFIX)/$(TARGET)'
    $(MAKE) -C '$(1)'
    $(MAKE) -C '$(1)' install
endef

