exports.xy2rad = function (x, y) {
	let rad = Math.atan(y / x)
	// 第二/三象限
	if (x < 0) {
		rad+= Math.PI
	}
	// 第四象限
	else if (y < 0) {
		rad+= 2 * Math.PI
	}
	return rad
}