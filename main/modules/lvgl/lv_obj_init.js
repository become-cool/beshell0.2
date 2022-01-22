beapi.lvgl.__lv_obj_init = function () {
	this._handles = {
		"#EVENT.ADD#": [n=>{
			this.enableEvent(n == "*" ? "all" : n)
		}],
		"#EVENT.CLEAR#": [n=>{
			this.disableEvent(n == "*" ? "all" : n)
		}]
	}
}; // xxd -i lv_obj_init.js > lv_obj_init.js.c