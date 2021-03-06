//*************************************************************************
//	创建日期:	2015-1-8
//	文件名称:	Emboss_Frag.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
// 浮雕
static const char* ShaderEmbossFrag = 
	"																\n\
	#ifdef GL_ES													\n\
	precision mediump float;										\n\
	precision mediump int;											\n\
	#endif															\n\
	\n\
	uniform sampler2D u_Texture;									\n\
	const vec2 texOffset = vec2( 0.005, 0.005);						\n\
	\n\
	varying vec4 v_fragmentColor;									\n\
	varying vec2 v_texCoord;										\n\
	\n\
	const vec4 lumcoeff = vec4(0.299, 0.587, 0.114, 0);				\n\
	\n\
	void main() 													\n\
	{																\n\
	vec2 tc0 = v_texCoord.st + vec2(-texOffset.s, -texOffset.t);	\n\
	vec2 tc1 = v_texCoord.st + vec2(         0.0, -texOffset.t);	\n\
	vec2 tc2 = v_texCoord.st + vec2(-texOffset.s,          0.0);	\n\
	vec2 tc3 = v_texCoord.st + vec2(+texOffset.s,          0.0);	\n\
	vec2 tc4 = v_texCoord.st + vec2(         0.0, +texOffset.t);	\n\
	vec2 tc5 = v_texCoord.st + vec2(+texOffset.s, +texOffset.t);			\n\
	\n\
	vec4 col0 = texture2D(u_Texture, tc0);									\n\
	vec4 col1 = texture2D(u_Texture, tc1);									\n\
	vec4 col2 = texture2D(u_Texture, tc2);									\n\
	vec4 col3 = texture2D(u_Texture, tc3);									\n\
	vec4 col4 = texture2D(u_Texture, tc4);									\n\
	vec4 col5 = texture2D(u_Texture, tc5);									\n\
	\n\
	vec4 sum = vec4(0.5) + (col0 + col1 + col2) - (col3 + col4 + col5);		\n\
	float lum = dot(sum, lumcoeff);											\n\
	gl_FragColor = vec4(lum, lum, lum, 1.0) * v_fragmentColor;  			\n\
	}";