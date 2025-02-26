module.exports = {
  content: ['./web_root/*.{html,js}'],
  xplugins: [ 'tailwindcss', 'xautoprefixer' ],
  corePlugins: {outline: false},
  theme: {
    extend: {},
    fontFamily: {
      sans:
      [
        "Inter var, Helvetica, sans-serif", {
          fontFeatureSettings: '"cv11", "ss01"',
          fontVariationSettings: '"opsz" 32',
        }
      ]
    }
  }
}
