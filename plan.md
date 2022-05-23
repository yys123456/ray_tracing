#### 思路

场景描述 -> 路径追踪器 -> 出图保存

场景描述为外部数据文件（包括json，贴图，网格.obj）

#### 场景描述

暂定使用json作为场景描述格式，整个场景scene包括网格mesh，隐函数表面surface, 和相机camera, 和环境env

##### mesh是一个对象数组

```json
"mesh":[
	{
        "mesh": "xxx.obj",
        "scale": [a, b, c],
        "translate": [0, 0, 0],
        "rotate": [a, b, c, d],
        "texture": "xxx.png", // 不一定有
        "material": {
        	"type": "",
            "param1": "",
            "param2": "",
            ...
    	}
    },
    {
        
    }
]
```

##### surface是一个对象数组

```json
"surface":[
	{
        "type": "sphere",
        "scale": [a, b, c],
        "translate": [0, 0, 0],
        "rotate": [a, b, c, d],
        "texture": "xxx.png",
        "material": {
        	"type": "",
            "param1": "",
            "param2": "",
            ...
    	}
    },
    {
        
    }
]
```

##### 相机是一个对象

目前只考虑单相机

```
"camera": {
	"type": "perspective",
	"target":[x, y, z],
	"origin":[x, y, z],
	"world_up": [0, 1, 0],
	"width": 1920,
	"height": 1080,
	"fov": 45,
	"near": 0.1,
	"far": 100
}
```

#### V1.0

不支持贴图

#### 类设计

##### Mesh类



