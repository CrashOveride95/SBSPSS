/*********************/
/*** Layer Trigger ***/
/*********************/

#include	<Davelib.h>
#include	<List2d.h>

//#include	"MkLevel.h"
#include	"MkLevelLayerTrigger.h"


/*****************************************************************************/
/*****************************************************************************/
/*** Pre-Process *************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CMkLevelLayerTrigger::PreProcess(CMkLevel *Core)
{
}

/*****************************************************************************/
/*****************************************************************************/
/*** Process *****************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CMkLevelLayerTrigger::Process(CMkLevel *Core)
{
		ProcessList(Core);
		printf("%i Trigger\n",ThingList.size());

}

/*****************************************************************************/
/*****************************************************************************/
/** Write ********************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int		CMkLevelLayerTrigger::Write(FILE *File,const char *LayerName,const char *MapName)
{
int			ThisPos=ftell(File);
sThingHdr	Hdr;
int			i,ListSize=ThingList.size();
		
		Hdr.Count=ListSize;
		fwrite(&Hdr,sizeof(sThingHdr),1,File);

		for (i=0; i<ListSize; i++)
		{
			sMkLevelLayerThing	&ThisThing=ThingList[i];
			sThingTrigger		OutThing;

			OutThing.Type=ThisThing.Type;
			OutThing.Pos.X=ThisThing.XY[0].x;
			OutThing.Pos.Y=ThisThing.XY[0].y;
			OutThing.Width=ThisThing.Data.Width;
			OutThing.Height=ThisThing.Data.Height;
			fwrite(&OutThing,sizeof(sThingTrigger),1,File);
		}

		return(ThisPos);
}

/*****************************************************************************/
