/***********************/
/*** Anim Base Class ***/
/***********************/

#ifndef	__FX_FX_BASE_ANIM_HEADER__
#define __FX_FX_BASE_ANIM_HEADER__

#include "fx/fx.h"

/*****************************************************************************/
class CFXBaseAnim : public CFX
{
public:
	struct sFXAnim
	{
		s16				Scale;
		s16				StartFrame,EndFrame;
		s16				FrameShift;
		u8				R,G,B;
		u8				Flags;
		DVECTOR			Velocity;
		u16				EndFX;
	};
	enum FXAnim_Flags
	{
		FXANIM_FLAG_LOOP			=1<<0,
		FXANIM_FLAG_COLLIDE_KILL	=1<<1,
		FXANIM_FLAG_HAS_GRAVITY		=1<<2,
	};

virtual void		init(DVECTOR const &Pos);
virtual void		shutdown();
virtual void		think(int _frames);
virtual void		render();
virtual	void		killFX();

		void		setData(void *Data)		{DataPtr=(sFXAnim*)Data;}

virtual	void		SetScale(int S)			{CurrentScale=S;}

protected:
		sFXAnim		*DataPtr;

		s16			Gravity;
		s16			MaxFrame;
		s16			CurrentFrame;
		s16			CurrentScale;
};

#endif
