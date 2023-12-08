module.exports = {
  content: ['./*.{html,js}'],
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
