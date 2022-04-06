const WidgetsMeta = require("../lv_widgets.meta.js")

let mapWidgets = {}
for(let w of WidgetsMeta) {
    mapWidgets[w.className] = w
}

module.exports = WidgetsMeta