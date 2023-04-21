> 本页中的绘图函数, 都有一个 `clip` 参数, 该参数在触发 Obj 的 `main-draw` 事件时会由 LVGL 传入


# 类 Path

Path 类可用于绘制特殊的矢量图像

## 方法


### begin(): void

开始定义路径

### end(): void

定义路径结束, 起点和终点如果不重合会自动闭合

### moveTo(x:number, y:number): void

移动到坐标 (x,y), 从该处绘制后续路径

### lineTo(x:number, y:number): void

绘制直线到坐标 (x,y)

### arcTo(x:number, y:number, cx:number, cy:number, clockwish:bool): void

从当前位置, 以(cx,cy)为圆心, 绘制圆弧到坐标 (x,y).

参数 clockwish 表示顺时针

### addLine(x1:number, y1:number, x2:number, y2:number): void

在路径上添加一段线段


### addLine(x1:number, y1:number, x2:number, y2:number, cx:number, cy:number): void

在路径上添加一段圆弧

参数:

* x1:number
 起点坐标x
 
* y1:number
 起点坐标y

* x2:number
 终点坐标x
 
* y2:number
 终点坐标y

* cx:number
 圆心坐标x
 
* cy:number
 圆心坐标y


### fill(x:number, y:number, zoom:float, color:lv_color_t, clip:lv_area_t): void

填充路径围成的图像, 起点和终点如果不重合会自动闭合

参数:

* x:number
 起点坐标x
 
* y:number
 起点坐标y

* zoom:float
 缩放比例
 
* color:lv_color_t
 颜色

* clip:lv_area_t
 可绘制范围,超出范围会被忽略


### stroke(x:number, y:number, zoom:float, color:lv_color_t, clip:lv_area_t): void

沿路径绘制线条

参数:

* x:number
 起点坐标x
 
* y:number
 起点坐标y

* zoom:float
 缩放比例
 
* color:lv_color_t
 颜色

* clip:lv_area_t
 可绘制范围,超出范围会被忽略


# 全局函数

${AUTO GEN}
