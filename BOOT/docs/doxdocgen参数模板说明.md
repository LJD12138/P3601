# Doxdocgen 参数模板占位符说明

## 已完成的配置

已在 `settings.json` 中配置参数模板：

```json
"doxdocgen.generic.paramTemplate": "@param{indent:8}{param} {commentary}{indent:8}",
"doxdocgen.generic.splitCasingSmartText": true,
```

## 占位符说明

### 可用的占位符

| 占位符 | 说明 | 示例 |
|--------|------|------|
| `{param}` | 参数名 | `obj` |
| `{type}` | 参数类型 | `SwitchObject_E` |
| `{name}` | 智能文本（驼峰拆分） | `switch object e` |
| `{commentary}` | 智能生成的描述 | `the obj` 或 `the switch object e` |

### 当前配置效果

对于函数：
```c
s8 cBms_Switch(SwitchObject_E obj, SwitchType_E type)
```

生成的注释为：
```c
----- @param obj the obj 
----- @param type the type 
```

## 优化建议

### 方案1：使用 {name} 占位符（更智能）

修改配置为：
```json
"doxdocgen.generic.paramTemplate": "@param{indent:8}{param} the {name}{indent:8}",
```

**生成效果：**
```c
----- @param obj the switch object e 
----- @param type the switch type e 
```

### 方案2：使用 {type} 占位符（显示类型）

修改配置为：
```json
"doxdocgen.generic.paramTemplate": "@param{indent:8}{param} ({type}){indent:8}",
```

**生成效果：**
```c
----- @param obj (SwitchObject_E)
----- @param type (SwitchType_E)
```

### 方案3：混合使用（推荐）

修改配置为：
```json
"doxdocgen.generic.paramTemplate": "@param{indent:8}{param} {type} {commentary}{indent:8}",
```

**生成效果：**
```c
----- @param obj SwitchObject_E the obj 
----- @param type SwitchType_E the type 
```

### 方案4：简洁明了（最推荐）

修改配置为：
```json
"doxdocgen.generic.paramTemplate": "@param{indent:8}{param} {name}{indent:8}",
```

**生成效果：**
```c
----- @param obj switch object e 
----- @param type switch type e 
```

## 完整配置示例

### 推荐配置（方案4）

```json
{
    "doxdocgen.generic.paramTemplate": "@param{indent:8}{param} {name}{indent:8}",
    "doxdocgen.generic.splitCasingSmartText": true
}
```

### 完整注释效果

对于函数：
```c
s8 cBms_Switch(SwitchObject_E obj, SwitchType_E type)
```

生成：
```c
/*********************************************************************************************************************
----- @brief 
----- @param obj switch object e 
----- @param type switch type e 
----- @return s8 
----- @author LJD (291483914@qq.com)
----- @date 2026-01-09
******************************************************************************************************************/
```

### 手动添加枚举值说明

在生成的基础上，手动编辑添加枚举值：

```c
/*********************************************************************************************************************
----- @brief 开关控制函数
----- 
----- @param[in] obj switch object e
-----                  - SO_KEY:   按键控制
-----                  - SO_CONSOLE: 面板控制
-----                  - SO_PARA:   并机控制
----- @param[in] type switch type e
-----                  - ST_NULL:  取反
-----                  - ST_ON:    打开
-----                  - ST_OFF:   关闭
----- @return s8 
----- 
----- @author LJD (291483914@qq.com)
----- @date 2026-01-09
******************************************************************************************************************/
```

## 立即测试

请在 VS Code 中重新加载配置（Ctrl+Shift+P -> "Reload Window"），然后在 `cBms_Switch` 函数上方按 `/` 键测试。

## 配置对比

| 配置 | 生成效果 | 优点 | 缺点 |
|------|----------|------|------|
| `{param} {commentary}` | `obj the obj` | 简单 | 描述不够详细 |
| `{param} the {name}` | `obj the switch object e` | 智能拆分 | 稍显冗余 |
| `{param} ({type})` | `obj (SwitchObject_E)` | 显示类型 | 无描述 |
| `{param} {name}` | `obj switch object e` | 简洁智能 | 需要理解驼峰 |

**建议使用：** `{param} {name}` （方案4）