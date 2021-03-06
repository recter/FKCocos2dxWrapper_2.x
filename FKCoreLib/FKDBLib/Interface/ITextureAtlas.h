//*************************************************************************
//	创建日期:	2014-8-13   16:46
//	文件名称:	ITextureAtlas.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "../Include/BaseMacro.h"
#include "../Include/Object.h"
#include "../Include/Rect.h"
#include <string>
using std::string;
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

class ITextureAtlas : public IObject
{
public:
	// TextureAtlas的名字
	virtual const string&	GetName() = 0;
	// 获取指定子资源所占面积区域
	virtual CRect			GetRegion( const string& p_szName ) = 0;
	// 资源清理
	virtual void			Clear() = 0;
};

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------