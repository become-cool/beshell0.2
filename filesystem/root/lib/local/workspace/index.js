const Workspace = require('./Workspace')
module.exports = Workspace.singleton()

global.workspace = module.exports