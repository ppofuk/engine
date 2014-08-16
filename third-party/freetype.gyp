{
  'includes': [
    '../common.gypi'
  ],
  # Thanks to thoes at: https://github.com/fire/polycode-gyp
  'targets': [
    {
      'target_name': 'freetype',
      'type': 'static_library',
       #'standalone_static_library': 1,
      'sources': [
        # base components (required)
        'freetype/src/base/ftsystem.c',
        'freetype/src/base/ftinit.c',
        'freetype/src/base/ftdebug.c',
        'freetype/src/base/ftbase.c',

        'freetype/src/base/ftbbox.c',       # recommended, see <freetype/ftbbox.h>
        'freetype/src/base/ftglyph.c',      # recommended, see <freetype/ftglyph.h>

        'freetype/src/base/ftbitmap.c',     # optional, see <freetype/ftbitmap.h>
        'freetype/src/base/ftfstype.c',     # optional
        'freetype/src/base/ftgasp.c',       # optional, see <freetype/ftgasp.h>
        'freetype/src/base/ftlcdfil.c',     # optional, see <freetype/ftlcdfil.h>
        'freetype/src/base/ftmm.c',         # optional, see <freetype/ftmm.h>
        'freetype/src/base/ftpatent.c',     # optional
        'freetype/src/base/ftstroke.c',     # optional, see <freetype/ftstroke.h>
        'freetype/src/base/ftsynth.c',      # optional, see <freetype/ftsynth.h>
        'freetype/src/base/fttype1.c',      # optional, see <freetype/t1tables.h>
        'freetype/src/base/ftwinfnt.c',     # optional, see <freetype/ftwinfnt.h>
        'freetype/src/base/ftxf86.c',       # optional, see <freetype/ftxf86.h>

        # font drivers (optional; at least one is needed)
        'freetype/src/cff/cff.c',           # CFF/OpenType font driver
        'freetype/src/sfnt/sfnt.c',         # SFNT files support (TrueType & OpenType)
        'freetype/src/truetype/truetype.c', # TrueType font driver

        # rasterizers (optional; at least one is needed for vector formats)
        'freetype/src/raster/raster.c',     # monochrome rasterizer
        'freetype/src/smooth/smooth.c',     # anti-aliasing rasterizer

        # auxiliary modules (optional)
        'freetype/src/autofit/autofit.c',   # auto hinting module
        'freetype/src/psaux/psaux.c',       # PostScript Type 1 parsing
        'freetype/src/pshinter/pshinter.c', # PS hinting module
        'freetype/src/psnames/psnames.c',   # PostScript glyph names support
      ],
      'include_dirs': [
        'freetype/include',
      ],
      'defines': [
        'FT2_BUILD_LIBRARY',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          'freetype/include',
        ],
      },
      'conditions': [
        [ 'OS == "mac"', {
          'sources': [
            'freetype/src/base/ftmac.c',        # only on the Macintosh
          ],
        }],
      ],
    },
  ],
}
