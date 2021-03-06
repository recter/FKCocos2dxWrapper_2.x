//*************************************************************************
//	创建日期:	2014-11-28
//	文件名称:	FKCW_RenderEx_ScrollingBackgroundNode.h
//  创 建 人:   王宏张 FreeKnight	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "FKCW_RenderEx_Macros.h"
//-------------------------------------------------------------------------
typedef std::list<CCSprite*> NodeList;
typedef std::list<CCSprite*>::iterator ListIter;

//current ：正方向上先离开屏幕的地图精灵
//follow  ：负方向上先离开屏幕的地图精灵
//leave   ：刚刚离开屏幕的地图精灵
typedef void (CCObject::*SEL_BgSpriteChangeEvent)(CCSprite* current,CCSprite* follow,CCSprite* leave);
#define bgSpriteChangeEvent(_SELECTOR) (SEL_BgSpriteChangeEvent)(&_SELECTOR)

//current ：正方向上先离开屏幕的地图精灵
//follow  ：负方向上先离开屏幕的地图精灵
typedef void (CCObject::*SEL_BgSpriteMovbEvent)(CCSprite* current,CCSprite* follow);
#define bgSpriteMoveEvent(_SELECTOR) (SEL_BgSpriteMovbEvent)(&_SELECTOR)

///背景滚动的方向
enum ENUM_ScrollDirection
{
	SD_RIGHT_TO_LEFT,
	SD_LEFT_TO_RIGHT,
	SD_TOP_TO_BOTTOM,
	SD_BOTTOM_TO_TOP
};
//-------------------------------------------------------------------------
/**
 * 滚动背景
 * 可以选择从左到右,从右到左,从上到下,从下到上四个方向滚动
 * 基本模式是无限循环,通过设置回调函数,可以实现边滚动边销毁画面或者插入画面
 * 先用addBackgroundBlock增加画面,再用开始滚动
 * 至少需要两个画面才能正确运行
 */
class ScrollingBackground : public CCNode
{
public:
	ScrollingBackground();
	virtual ~ScrollingBackground();

	// 增加背景画面,传入的节点会自动铺满整个屏幕,并加到链表尾
	// 注：可以一边滚动一边增加背景画面节点,如果当前节点是链表尾,会失败并返回false
	bool addBackgroundSprite(CCSprite* bgSprite);
	// 开始连续滚动
	bool beginContinuousScroll(float velocity);
	// 背景滚动到两个背景节点中的一个彻底离开屏幕
	bool scrollToNextBgSprite(float velocity);
	// 设置背景移动设定的距离
	bool setBgSpriteScrollBy(float velocity,float distance);
	// 暂停滚动
	void pauseScroll();
	// 恢复滚动
	void resumeScroll(float velocity);
	// 移除背景节点,但必须保证节点链表内的节点数大于2,且不能移除当前显示在屏幕内的两个节点
	bool removeBgSprite(CCSprite* bgSprite);
	// 设置背景全屏完整显示
	void setFullScreen(CCSprite* bgSprite);
	// 返回地图链表内有多少张地图画面
	int   getNumBgSprite();

	float		getVelocity();
	bool		isListBack(CCSprite* bgBlock);
	bool		isListFront(CCSprite* bgBlock);
	void		setVelocity(float velocity);

	CCSprite*	getBackBgSprite();
	CCSprite*	getFrontBgSprite();
	CCSprite*	getCurBgSprite();
	CCSprite*	getFollowBgSprite();
	
	/**
	 * 当一个画面彻底离开屏幕的时候会发送这个消息
	 * SEL_BgSpriteChangeEvent 的 CCSprite* 参数传的是屏幕内的两个地图节点和一个已经离开屏幕的地图节点
	 */
	void setBgChangeEventListener(CCObject *target, SEL_BgSpriteChangeEvent selector);

	/**
	 * 当一个画面移动的时候会发送这个消息
	 * SEL_BgSpriteChangeEvent 的 CCSprite* 参数传的是屏幕内的两个地图节点
	 */
	void setBgMoveEventListener(CCObject *target, SEL_BgSpriteMovbEvent selector);

	/**
	 *  一个静态方法,用于创建ScrollingBackground
	 *@param type 滚动模式
	 *@param ScrollDirection 滚动方向 
	 */
	static ScrollingBackground* create(ENUM_ScrollDirection direction);
protected:
	ENUM_ScrollDirection	m_direction;     // 滚动方向
	NodeList				m_bgSpriteList;  // 节点储存链表
	ListIter				m_curIter;       // 当前出现在屏幕上的两个画面的前一个（正方向先滚出屏幕的那个）
	ListIter				m_followIter;    // 当前出现在屏幕上的两个画面的后一个，跟随画面（正方向后滚出屏幕的那个）
	CCSize			m_visibleSize;   // 储存屏幕的大小,用于将各个画面缩放到铺满屏幕
	bool					m_isScrolling;   // 是否正在滚动
	bool					m_bScrollToNext; // 是否设置了背景滚动到两个背景节点中的一个彻底离开屏幕
	bool					m_bScrollBy;     // 是否设定背景滚动一定距离
	float					m_velocity;      // 滚动的速度
	float					m_distance;      // 设定背景块的移动距离
	float					m_sensitivity;   // 调用scrollToNextBgSprite方法滚动到下一张图片的识别灵敏度
	CCObject*      m_changeEventListener;
    SEL_BgSpriteChangeEvent m_changeEventSelector;
	CCObject*      m_moveEventListener;
    SEL_BgSpriteMovbEvent   m_moveEventSelector;

protected:
	bool init(ENUM_ScrollDirection direct);
	// 创建节点并将节点插入列表
	void	_AddListNode(CCSprite* bgSprite);
	// 一个画面彻底离开屏幕的时候调用它来,重新设置 m_curIter 和 m_followIter
	void	_ChangeCurBgSprite();
	// 每一帧调用一次,设置画面的位置
	void	_MoveAction(float d);
	// 设置跟随画面的位置
	void	_SetFollowNodePosition();
	// 判断一个画面是否不在屏幕范围内
	bool	_IsChangeBg(const CCPoint& posCur,const CCPoint& achorPointCur);
	// 在速度为负的时候,要先设置下一时刻 m_curIter 所指向的背景节点的位置
	void	_PreSetCurPos();
	// 滚动的时候的公共设置
	bool	_BeginScroll(float velocity);
};
