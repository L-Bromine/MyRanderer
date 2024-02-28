1.程序介绍
该渲染器由opengl框架实现，其中渲染实现部分未调用绘制函数，仅调用向缓冲区复制内存函数。
对于每一个面的图元，渲染流程为：
坐标变换 – 背面剔除 – 裁剪 – 生成像素参数列表 – 应用像素shader函数 – zbuffer面消隐 – 绘制
所有图元继承自Element虚基类，需实现 void draw（）接口。如需实现路径追踪，需实现bool triangleIntersect(const Ray& ray, float* out)接口。
对于所有图元由Scence类统一管理，统一渲染。
程序由cpu单线程渲染，每个面独立渲染流，顺序渲染。
程序实现天空盒与粒子系统。天空盒纹理为bkimg.dat文件。
程序内使用纹理为处理后纹理，会在附件中给出python处理程序。
程序内可导入obj文件。
程序未实现全局光照与阴影，但留有光线追踪接口与绘制函数，并部分实现路径追踪。
2.程序运行介绍：
	操作方式：通过wsad进行前后左右移动摄像机，空格上升摄像机，z键下降摄像机，鼠标移动控制视角，1 2 3按键控制改变所观看的场景。
	
	场景会在切换时加载，故场景2会在切换时加载较长时间。
	场景1是开启时默认的场景，包含一个球、一个由六个面组成的立方体、天空盒与一个光源。球由obj读取，为平滑着色，立方体在程序中定义，由六个面组成，其中一个面为透明面，一个面包含纹理。该立方体为平直着色，
	场景2是读取大obj文件场景，包含一个约3w面的人物模型，包含纹理与光照。模型文件与贴图文件较大，读取需较长时间。
	场景3是粒子系统展示场景，展示了包含重力模拟的五个不同颜色的粒子系统。

3.程序类结构
	Matrix类：矩阵的处理类，包含矩阵与矩阵的基本运算和矩阵与数的基本运算，以及矩阵求逆等常见操作。
	Ray类：射线类，处理射线与图元的相交。
	Light类：存储光照光源信息的类结构，不包含光照处理。
	Camera类：摄像机类，负责处理摄像机变换等变换，并记录摄像机位置，方向角与变换矩阵。
	Element类：
		Element类是所有图元的基类，包含点阵，材质，uv，法线等参数，整个图元绘制流程应在draw（）中定义。
	Point类：
		继承自Element类，是点的绘制。
	Line类：
		继承自Element类，是线的绘制类。
	Triagle类：
		继承自Element类，是三角形图元的绘制类。
	Polygon类：
		继承自Element类，是凸多边形图元的绘制类，允许对每个像素应用shader函数。
	ObjObject类：
		继承自Element类，是对.obj文件的读取后形成的对象类，允许对每个像素应用shader函数。
	Particle类：
		继承自Element类，是粒子系统的绘制和模拟类。
	SkyBox类：
		继承自Element类，是天空盒绘制类，读取bkimg.dat产生背景图片。

Texture类*：
	纹理类，存储一个处理好的纹理，并通过uv读取颜色。	

Scence类：
	是场景类，保存一个场景中的每一个图元、光源与摄像机，存储帧缓存与变换栈，处理整个场景的绘制与消隐并在每一帧绘制结束的时候将色彩缓存输入显存。

MyException及其子类：
	异常类，自定义异常。

ExceptionHanding类：
	异常处理类，使用try-catch类捕获MyException类并处理。


下面是Element基类的接口定义：
	/// 初始化图元变换及顶点组，x为点的个数
	Element(int x);

	/// 设置uv
	void setUV(float* uv);

	/// 设置法向
	void setNormals(float* normals);

	/// 设置材质
	void setTexture(Texture*);

	/// 通过4x4矩阵重置tranform
	void tranform(const Matrix&);

	/// 在3D坐标上移动图元
	void move(float x, float y, float z = 0);

	/// 欧拉角旋转
	void rotate(float x, float y, float z);

	/// 四元数旋转
	void rotate(float x, float y, float z, float w);

	/// 在某轴上旋转一个角度（弧度制）
	void axisRotate(float n, float x, float y, float z);

	/// 缩放物体
	void scale(float x, float y, float z);

	/// 设置颜色数组
	void setColors(float* cs);

	/// 拷贝颜色矩阵
	void setColors(const Matrix& cs);

	/// 以scence为父级变换
	Matrix tranform() const;

	/// 返回平均透明度
	virtual float averageAlpha()const;

	/// 设置额外参数
	void setParm(int parmm, float* parm);

	/// 射线三角形相交接口
	virtual bool triangleIntersect(const Ray& ray, float* out) const;

	/// 绘制接口
	virtual void draw() const = 0;

注：Texture类需读取处理好的纹理文件，下附处理纹理的python程序：
def readimg(dirpath, outpath="output.txt"):
    img_plt = plt.imread(dirpath)

    with open(outpath, "w") as file:
        a = img_plt.shape
        file.write(str(a[0]) + " " + str(a[1]) + " " + str(a[2]) + "\n")

        for i in img_plt:
            for j in i:
                if a[2] == 3:
                    file.write(str(j[2]) + " " + str(j[1]) + " " + str(j[0]) + "\n")
                elif a[2] == 4:
                    file.write(str(j[2]) + " " + str(j[1]) + " " + str(j[0]) + " " + str(j[3]) + "\n")



3.操作方式：
wsad前后左右移动
z向下移动
空格向上移动
123切换场景
鼠标控制视角