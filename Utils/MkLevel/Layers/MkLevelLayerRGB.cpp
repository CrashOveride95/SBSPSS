/*****************/
/*** Layer RGB ***/
/*****************/

#include	<DaveLib.h>
#include	<List2d.h>

#include	"MkLevelLayer.h"
#include	"MkLevelLayerRGB.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CMkLevelLayerRGB::CMkLevelLayerRGB(sExpLayerHdr *LayerHdr)
{
int		*iPtr;
u8		*Ptr=(u8*)LayerHdr;

		Type=LayerHdr->Type;
		SubType=LayerHdr->SubType;
		Width=LayerHdr->Width;
		Height=LayerHdr->Height;

		RGBMap.SetSize(Width,Height);

		iPtr=(int*)(Ptr+sizeof(sExpLayerHdr));

		ShadeFlag=*iPtr++;

sRGBCol	*RGB=(sRGBCol*)iPtr;
		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sRGBElem	ThisElem;
				ThisElem.RGB=*RGB++;
				ThisElem.RGB.P=0;
				ThisElem.TableIdx=0;
//				ThisElem.RGB.R&=-8;
//				ThisElem.RGB.G&=-8;
//				ThisElem.RGB.B&=-8;
				RGBMap.Set(X,Y,ThisElem);
				
			}
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*** Pre-Process *************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define	NUM_OF_PAL_ENTRIES	256
#include	<tquant.h>
void	CMkLevelLayerRGB::PreProcess(CMkLevel *Core)
{
int				Size=Width*Height;
int				X,Y,Ofs;
vector<u8>		SourcePal;
vector<u8>		DestPal;
vector<u8>		RemapMap;

		SourcePal.resize(Width*Height*3);
		RemapMap.resize(Width*Height);
		DestPal.resize(NUM_OF_PAL_ENTRIES*3);
		
		Ofs=0;
		for (Y=0; Y<Height; Y++)
		{
			for (X=0; X<Width; X++)
			{
				sRGBElem	&ThisElem=RGBMap.Get(X,Y);
				SourcePal[Ofs++]=ThisElem.RGB.R;
				SourcePal[Ofs++]=ThisElem.RGB.G;
				SourcePal[Ofs++]=ThisElem.RGB.B;
			}
		}

	    tquant(&SourcePal[0],&RemapMap[0],&DestPal[0],NUM_OF_PAL_ENTRIES,Width*Height);



// Build Out Map And get max color
int		ColorCount=0;
		OutMap.SetSize(Width,Height);
		Ofs=0;
		for (Y=0; Y<Height; Y++)
		{
			for (X=0; X<Width; X++)
			{
				sRGBElem	&ThisElem=RGBMap.Get(X,Y);
				
				u8	Idx=RemapMap[Ofs++];
				OutMap.Set(X,Y,Idx);
				if (ColorCount<Idx) ColorCount=Idx;
//				if (ThisElem.RGB.R!=DestPal[(Idx*3)+0] || ThisElem.RGB.G!=DestPal[(Idx*3)+1] || ThisElem.RGB.B!=DestPal[(Idx*3)+2])	printf("%3d %3d %3d -> %3d %3d %3d\n",ThisElem.RGB.R,ThisElem.RGB.G,ThisElem.RGB.B,DestPal[(Idx*3)+0],DestPal[(Idx*3)+1],DestPal[(Idx*3)+2]);
			}
		}
		ColorCount++;

		printf("RGB remapped to %i colors\n",ColorCount);
// Build Out RGB Table
		OutRGBTable.SetSize(ColorCount,16);
int		RGBInc=6;
		if (ShadeFlag) RGBInc=-6;
		for (int c=0;c<ColorCount;c++)
		{
			int		R=DestPal[(c*3)+0];
			int		G=DestPal[(c*3)+1];
			int		B=DestPal[(c*3)+2];
			for (int s=0; s<16; s++)
			{
				sRGBCol	RGB;
				RGB.R=R; RGB.G=G; RGB.B=B;
				OutRGBTable.Set(c,s,RGB);

				R+=RGBInc;
				G+=RGBInc;
				B+=RGBInc;
				if (R<0) R=0; else if (R>255) R=255;
				if (G<0) G=0; else if (G>255) G=255;
				if (B<0) B=0; else if (B>255) B=255;
			}
		}
}
#if	0
void	CMkLevelLayerRGB::PreProcess(CMkLevel *Core)
{
int		X,Y;
int		ColorCount;
int		c;

// build RGB List
		for (Y=0; Y<Height; Y++)
		{
			for (X=0; X<Width; X++)
			{
				sRGBElem	&ThisElem=RGBMap.Get(X,Y);
				sRGBList	ThisRGB;
				ThisRGB.RGB=ThisElem.RGB;
				ThisRGB.Count=0;
				int	Idx=InRGBList.Add(ThisRGB);
				ThisElem.TableIdx=Idx;
				InRGBList[Idx].Count++;
			}
		}

		ColorCount=InRGBList.size();
		printf("------ %i Colors ------\n",ColorCount);

// Sort RGB List by usage
		for (c=0; c<ColorCount; c++)
		{
			sRGBList	&ThisRGB=InRGBList[c];
			int	s=0;
			int	SortListSize=SortRGBList.size();
			for (; s<SortListSize; s++)
			{
				if (SortRGBList[s].Count<ThisRGB.Count) break;

			}
			SortRGBList.insert(s,ThisRGB);
		}
// Build Remap table
		for (c=0; c<ColorCount; c++)
		{
			int	Idx=SortRGBList.Find(InRGBList[c]);
			RemapTable.push_back(Idx);
/*
			sRGBCol	&In=InRGBList[c].RGB;
			sRGBCol	&Out=SortRGBList[RemapTable[c]].RGB;
			if (In.R!=Out.R || In.G!=Out.G || In.B!=Out.B)
			{
				printf("%i %i %i\t%i %i %i\n",In.R,In.G,In.B,Out.R,Out.G,Out.B);
			}
*/
		}
// Remap all above 256
		for (c=256;c<ColorCount;c++)
		{
			sRGBCol	&ThisRGB=InRGBList[RemapTable[c]].RGB;
			RemapTable[c ]=FindClosestRGB(ThisRGB);
		}

// Build Out Map
		OutMap.SetSize(Width,Height);
		for (Y=0; Y<Height; Y++)
		{
			for (X=0; X<Width; X++)
			{
				sRGBElem	&ThisElem=RGBMap.Get(X,Y);
				
				u8	Idx=RemapTable[ThisElem.TableIdx];
				OutMap.Set(X,Y,Idx);
			}
		}

// Build Out RGB Table
		if (ColorCount>256) ColorCount=256;
		OutRGBTable.SetSize(ColorCount,16);
int		RGBInc=6;
		if (ShadeFlag) RGBInc=-6;
		for (c=0;c<ColorCount;c++)
		{
			sRGBCol	&ThisRGB=SortRGBList[c].RGB;
			int		R=ThisRGB.R;
			int		G=ThisRGB.G;
			int		B=ThisRGB.B;
			for (int s=0; s<16; s++)
			{
				sRGBCol	RGB;
				RGB.R=R; RGB.G=G; RGB.B=B;
				OutRGBTable.Set(c,s,RGB);

				R+=RGBInc;
				G+=RGBInc;
				B+=RGBInc;
				if (R<0) R=0; else if (R>255) R=255;
				if (G<0) G=0; else if (G>255) G=255;
				if (B<0) B=0; else if (B>255) B=255;
			}
		}

}

/*****************************************************************************/
int		CMkLevelLayerRGB::FindClosestRGB(sRGBCol const &RGB)
{
int		CIdx=-1;
int		CVal=0;

	for (int i=0; i<256; i++)
	{
		sRGBCol		&ListRGB=SortRGBList[i].RGB;

		int		Dr=abs(ListRGB.R-RGB.R);
		int		Dg=abs(ListRGB.G-RGB.G);
		int		Db=abs(ListRGB.B-RGB.B);
/*
		int	ThisVal=(SortRGBList[i].RGB.R*SortRGBList[i].RGB.R)+ 
					(SortRGBList[i].RGB.G*SortRGBList[i].RGB.G)+
					(SortRGBList[i].RGB.B*SortRGBList[i].RGB.B);
*/		
		int	ThisVal=Dr+Dg+Db;
					
		if (CIdx==-1 || ThisVal<CVal)
		{
			CIdx=i;
			CVal=ThisVal;
		}
	}
	return(CIdx);

}

#endif
/*****************************************************************************/
/*****************************************************************************/
/*** Process *****************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CMkLevelLayerRGB::Process(CMkLevel *Core)
{
}

/*****************************************************************************/
/*****************************************************************************/
/** Write ********************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int		CMkLevelLayerRGB::Write(CMkLevel *Core,FILE *File,const char *LayerName)
{
int				ThisPos=ftell(File);
sLayerHdr		Hdr;
sLayerRGBHdr	RGBHdr;

		Hdr.Type=Type;
		Hdr.SubType=SubType;
		Hdr.Width=Width;
		Hdr.Height=Height;
		fwrite(&Hdr,sizeof(sLayerHdr),1,File);

int		HdrPos=ftell(File);
		fwrite(&RGBHdr,sizeof(sLayerRGBHdr),1,File);

		RGBHdr.RGBMap=WriteRGBMap(File);
		PadFile(File);
		RGBHdr.RGBTable=WriteRGBTable(File);
		PadFile(File);
// rewrite header
int		RetPos=ftell(File);
		fseek(File,HdrPos,SEEK_SET);
		fwrite(&RGBHdr,sizeof(sLayerRGBHdr),1,File);
		fseek(File,RetPos,SEEK_SET);
		Size=ftell(File)-ThisPos;

		return(ThisPos);
}

/*****************************************************************************/
int		CMkLevelLayerRGB::WriteRGBMap(FILE *File)
{
int		ThisPos=ftell(File);

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				u8	Idx=OutMap.Get(X,Y);
				fwrite(&Idx,1,sizeof(u8),File);
			}
		}
		printf("--> RGBMap %i %i\n",ThisPos,ftell(File)-ThisPos);
		return(ThisPos);
}
/*****************************************************************************/
int		CMkLevelLayerRGB::WriteRGBTable(FILE *File)
{
int		ThisPos=ftell(File);
int		ListSize=OutRGBTable.GetWidth();
		
		for (int c=0; c<ListSize; c++)
		{
			for (int s=0; s<16; s++)
			{
				sRGBCol	&RGB=OutRGBTable.Get(c,s);
				fwrite(&RGB,1,sizeof(sRGBCol),File);
			}
		}
		printf("--> RGBTable %i %i\n",ThisPos,ftell(File)-ThisPos);
		return(ThisPos);
}

