/*********************/
/*** Climax Macros ***/
/*********************/

#ifndef	__CMX_MACROS_HEADER__
#define	__CMX_MACROS_HEADER__


/*****************************************************************************/
// Get the summed magnitude of the vector XYZ (after sqr)
#define CMX_StVecXYZMag(r0) __asm__ volatile (		\
		"mfc2	$12, $25;"							\
		"mfc2	$13, $26;"							\
		"mfc2	$14, $27;"							\
		"add	$12, $12, $13;"						\
		"add	$12, $12, $14;"						\
		"sw		$12, 0( %0 );"						\
		:											\
		: "r"( r0 )									\
			: "$12", "$13", "$14", "memory" )

/*---------------------------------------------------------------------------*/
// Get the summed magnitude of the vector XZ (after sqr)
#define CMX_StVecXZMag(r0) __asm__ volatile (		\
		"mfc2	$12, $25;"							\
		"mfc2	$13, $27;"							\
		"add	$12, $12, $13;"						\
		"sw		$12, 0( %0 );"						\
		:											\
		: "r"( r0 )									\
		: "$12", "$13", "memory" )

/*---------------------------------------------------------------------------*/
// Load IR0,IR1,IR2 with values (for SQR)
#define CMX_ldXYZ(r0,r1,r2) __asm__  (			\
		"mtc2   %0,$9;"							\
		"mtc2   %1,$10;"						\
		"mtc2   %2,$11"							\
		:										\
		: "r"( r0 ),"r"( r1 ),"r"( r2 ) )

/*---------------------------------------------------------------------------*/
// Load IR0,IR1,IR2 with values (for SQR)
#define CMX_ldXZ(r0,r1) __asm__  (				\
		"mtc2   %0,$9;"							\
		"mtc2   %1,$11"							\
		:										\
		: "r"( r0 ),"r"( r1 ))

/*****************************************************************************/
/*** Smaller Translation Macros (no return flags) ****************************/
/*****************************************************************************/
#define CMX_RotTransPers(r1,r2)						\
{		gte_ldv0(r1);								\
		gte_rtps();									\
		gte_stsxy(r2);								\
}

/*---------------------------------------------------------------------------*/
#define CMX_RotTransPers3(r1,r2,r3,r4,r5,r6)		\
{		gte_ldv3(r1,r2,r3);							\
		gte_rtpt();									\
		gte_stsxy3(r4,r5,r6);						\
}

/*---------------------------------------------------------------------------*/
#define CMX_RotTransPers4(r1,r2,r3,r4,r5,r6,r7,r8)	\
{		gte_ldv0(r4);								\
		gte_rtps();									\
		gte_ldv3(r1,r2,r3);							\
		gte_stsxy(r8);								\
		gte_rtpt();									\
		gte_stsxy3(r5,r6,r7);						\
}

/*---------------------------------------------------------------------------*/
#define CMX_RotTrans(r1,r2)							\
{		gte_ldv0(r1);								\
		gte_rt();									\
		gte_stlvnl(r2);								\
}

/*****************************************************************************/
#endif