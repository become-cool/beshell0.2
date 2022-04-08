const WidgetsMeta = require("../lv_widgets.meta.js")

WidgetsMeta.map = {}
for(let w of WidgetsMeta) {
    WidgetsMeta.map[w.className] = w
}

module.exports = WidgetsMeta