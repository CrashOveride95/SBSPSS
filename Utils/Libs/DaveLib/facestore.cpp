/****************************************/
/*** Generic Face Compilation Storage ***/
/****************************************/
// Contains tri data with texture data
// Will quad later

#include <ginio.h>
#include <gintex.h>
#include <math.h>

#include "FaceStore.h"
#include "TexGrab.h"
#include "DaveLib.h"
#include "PsxTypes.h"
#include <algorithm>

//***************************************************************************
//vector<GString>			CFaceStore::TexList;
//CTexGrab				CFaceStore::FaceStoreTexGrab;
//CTexGrab				*CFaceStore::TexGrab=&FaceStoreTexGrab;

//***************************************************************************
Vector3 normalise(Vector3 &v)
{
	float sqmag = v.x * v.x + v.y * v.y + v.z * v.z;
	if (sqmag > 0.001f)
	{
		float mag = (float)sqrt( sqmag );
		Vector3 out;

		out.x = v.x / mag;
		out.y = v.y / mag;
		out.z = v.z / mag;

		return out;
	} else
	{
		return v;
	}
}

//***************************************************************************
Vector3 crossProduct( Vector3 &v0, Vector3 &v1, Vector3 &v2 )
{
	Vector3 dv1, dv2;
	Vector3 out;

	dv1.x = v1.x - v0.x;
	dv1.y = v1.y - v0.y;
	dv1.z = v1.z - v0.z;

	dv2.x = v2.x - v0.x;
	dv2.y = v2.y - v0.y;
	dv2.z = v2.z - v0.z;

	out.x = (dv1.z * dv2.y) - (dv1.y * dv2.z);
	out.y = (dv1.x * dv2.z) - (dv1.z * dv2.x);
	out.z = (dv1.y * dv2.x) - (dv1.x * dv2.y);

	out = normalise(out);
	return out;
}

//***************************************************************************
inline float dotProduct( Vector3 &v0, Vector3 &v1 )
{
	float dp = v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
	return dp;
}

//***************************************************************************
inline bool aprox( float x, float y )
{
	if (fabs(x-y)<0.03)
	{
		return true;
	} else
	{
		return false;
	}
}

//***************************************************************************
inline bool uvaprox( sUV &uv0, sUV &uv1 )
{
	return (aprox(uv0.u, uv1.u) && aprox(uv0.v, uv1.v));
}

//***************************************************************************
CFace	&CFaceStore::AddFace(vector<Vector3> const &P, const sGinTri &T, const sUVTri &uv,GString const &Tex,int ID)
{
//int		ListSize = FaceList.size();
//		FaceList.resize(ListSize+1);
//CFace	&F = FaceList[ListSize];
CFace	F;

		for (int i=0; i<3; i++)
		{
			F.pts[i] = T.p[i];
			F.vis[i] = T.vis[i];
			F.uvs[i] = uv.p[i];
			F.vtx[i] = P[T.p[i]];
// Limit UV's
			if (F.uvs[i].u < 0.f) F.uvs[i].u=0.f;
			if (F.uvs[i].u > 1.f) F.uvs[i].u=1.f;
			if (F.uvs[i].v < 0.f) F.uvs[i].v=0.f;
			if (F.uvs[i].v > 1.f) F.uvs[i].v=1.f;
		}

		F.TexName=Tex;
//		F.Mat = AddTex(Tex);
//		F.Normal = crossProduct( F.vtx[0], F.vtx[1], F.vtx[2] );
//		F.Avail = true;
//		F.ID=ID;
		
		return(AddFace(F));
}

//***************************************************************************
CFace	&CFaceStore::AddFace(CFace &Face)
{
int		ListSize=FaceList.size();
		FaceList.resize(ListSize+1);
CFace	&F = FaceList[ListSize];
		F=Face;

// Process Vtx's
		for (int i=0; i<3; i++)
		{
			F.pts[i]=AddVtx(F.vtx[i]*Scale);
		}

		F.Mat=AddTex(Face.TexName);
		F.Normal = crossProduct( F.vtx[0], F.vtx[1], F.vtx[2] );
		F.Avail = true;
		return(F);
}

//***************************************************************************
void	CFaceStore::AddFaces(vector<CFace> &Faces)
{
int		ListSize=Faces.size();

		for (int i=0 ;i<ListSize ;i++)
		{
			AddFace(Faces[i]);
		}
}

//***************************************************************************
void	CFaceStore::AddFaces(CFaceStore &Faces)
{
int		ListSize=Faces.GetFaceCount();

		for (int i=0 ;i<ListSize ;i++)
		{
			AddFace(Faces[i]);
		}
}

//***************************************************************************
CFace	&CFaceStore::AddFace(sTriFace &Face,int ID)
{
int		ListSize = FaceList.size();
		FaceList.resize(ListSize+1);
CFace	&F = FaceList[ListSize];

	for (int i=0; i<3; i++)
	{
		F.pts[i] = Face.pts[i];
		F.uvs[i] = Face.uvs[i];
		F.vtx[i] = Face.vtx[i];
// Limit UV's
		if (F.uvs[i].u < 0.f) F.uvs[i].u=0.f;
		if (F.uvs[i].u > 1.f) F.uvs[i].u=1.f;
		if (F.uvs[i].v < 0.f) F.uvs[i].v=0.f;
		if (F.uvs[i].v > 1.f) F.uvs[i].v=1.f;
	}

	F.Mat = Face.Mat;
	F.Normal = crossProduct( F.vtx[0], F.vtx[1], F.vtx[2] );
	F.Avail = true;
	F.ID=ID=0;
	return(F);
}

//***************************************************************************
//*** Texture Stuff *********************************************************
//***************************************************************************
int		CFaceStore::AddTex(GString const &TexName)
{
int		ListSize=TexList.size();
GString	Filename=TexName;
		Filename.Upper();

		for (int i=0; i<ListSize; i++)
		{
			if (TexList[i].Name==Filename) return(TexList[i].TexGrabId);
		}
// Does file exist
		if (!FindFile(TexName))
			GObject::Error(ERR_FATAL,"Texture not found %s!!\n",TexName);




		TexList.resize(ListSize+1);
sFaceTexList	&New=TexList[ListSize];

		New.Name=Filename;

//		printf("Add Tex %s\n",TexName);
		New.TexGrabId=TexGrab->AddFile(Filename);

		return(New.TexGrabId);
}

//***************************************************************************
void	CFaceStore::ProcessTextures()
{
// Set Texgrab Defaults		
		TexGrab->ShrinkToFit(true);
		TexGrab->NoSort();

		TexGrab->AnimatedHeadersOnly(true);
		TexGrab->DontOutputBoxes(true);
		TexGrab->AllowRotate(true);
		TexGrab->Process();
}

//***************************************************************************
void	CFaceStore::Process()
{
		Quad();
		BuildOutTriLists();
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
void	CFaceStore::Quad()
{
int		FaceCount=FaceList.size();

		for (int i=0; i<FaceCount; i++)
		{
			TriFaceList.push_back(FaceList[i]);
		}
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
void	CFaceStore::SetupUV(CFace const &In, sTri &Out)
{
vector<sTexOutInfo>	&TexInfo=TexGrab->GetTexInfo();
sTexOutInfo			&ThisTex=TexInfo[In.Mat];
			
int		W = ThisTex.w - 1;
int		H = ThisTex.h - 1;

		if (ThisTex.Rotated)
		{
			Out.uv0[0] = (ThisTex.u + H) - round(In.uvs[0].v * H);
			Out.uv0[1] = (ThisTex.v + W) - round(In.uvs[0].u * W);
			Out.uv1[0] = (ThisTex.u + H) - round(In.uvs[1].v * H);
			Out.uv1[1] = (ThisTex.v + W) - round(In.uvs[1].u * W);
			Out.uv2[0] = (ThisTex.u + H) - round(In.uvs[2].v * H);
			Out.uv2[1] = (ThisTex.v + W) - round(In.uvs[2].u * W);
		} 
		else
		{
			Out.uv0[0] = (ThisTex.u)+		round(In.uvs[0].u * W);
			Out.uv0[1] = (ThisTex.v + H) -	round(In.uvs[0].v * H);
			Out.uv1[0] = (ThisTex.u)+		round(In.uvs[1].u * W);
			Out.uv1[1] = (ThisTex.v + H) -	round(In.uvs[1].v * H);
			Out.uv2[0] = (ThisTex.u)+		round(In.uvs[2].u * W);
			Out.uv2[1] = (ThisTex.v + H) -	round(In.uvs[2].v * H);
		}

		Out.TPage=ThisTex.Tpage;
		Out.Clut=ThisTex.Clut;
}

//***************************************************************************
void	CFaceStore::SetupUV(CFace const &In, sQuad &Out)
{
vector<sTexOutInfo>	&TexInfo=TexGrab->GetTexInfo();
sTexOutInfo			&ThisTex=TexInfo[In.Mat];

int		W = ThisTex.w - 1;
int		H = ThisTex.h - 1;

		if (ThisTex.Rotated)
		{
			Out.uv0[0] = (ThisTex.u + H) - round(In.uvs[0].v * H);
			Out.uv0[1] = (ThisTex.v + W) - round(In.uvs[0].u * W);
			Out.uv1[0] = (ThisTex.u + H) - round(In.uvs[1].v * H);
			Out.uv1[1] = (ThisTex.v + W) - round(In.uvs[1].u * W);
			Out.uv2[0] = (ThisTex.u + H) - round(In.uvs[2].v * H);
			Out.uv2[1] = (ThisTex.v + W) - round(In.uvs[2].u * W);
			Out.uv3[0] = (ThisTex.u + H) - round(In.uvs[3].v * H);
			Out.uv3[1] = (ThisTex.v + W) - round(In.uvs[3].u * W);
		} 
		else
		{
			Out.uv0[0] = (ThisTex.u)+		round(In.uvs[0].u * W);
			Out.uv0[1] = (ThisTex.v + H) -	round(In.uvs[0].v * H);
			Out.uv1[0] = (ThisTex.u)+		round(In.uvs[1].u * W);
			Out.uv1[1] = (ThisTex.v + H) -	round(In.uvs[1].v * H);
			Out.uv2[0] = (ThisTex.u)+		round(In.uvs[2].u * W);
			Out.uv2[1] = (ThisTex.v + H) -	round(In.uvs[2].v * H);
			Out.uv3[0] = (ThisTex.u)+		round(In.uvs[3].u * W);
			Out.uv3[1] = (ThisTex.v + H) -	round(In.uvs[3].v * H);
		}

		Out.TPage=ThisTex.Tpage;
		Out.Clut=ThisTex.Clut;
}



//***************************************************************************
int		CFaceStore::AddVtx(Vector3 &InVtx)
{
int		ListSize=OutVtxList.size();
sVtx	ThisVtx;

		ThisVtx.vx=round(InVtx.x);
		ThisVtx.vy=round(InVtx.y);
		ThisVtx.vz=round(InVtx.z);

		for (int i=0; i<ListSize; i++)
		{
			if (OutVtxList[i]==ThisVtx) return(i);
		}

		OutVtxList.push_back(ThisVtx);
		return(ListSize);
}

//***************************************************************************
void	CFaceStore::BuildOutTriLists()
{
int		FaceCount=TriFaceList.size();

		OutTriList.resize(FaceCount);

		for (int i=0; i<FaceCount; i++)
		{
			CFace	&InFace=TriFaceList[i];
			sTri	&OutFace=OutTriList[i];

			OutFace.P0=InFace.pts[0];
			OutFace.P1=InFace.pts[1];
			OutFace.P2=InFace.pts[2];

// Materials and other shit
			SetupUV(InFace,OutFace);
		}

}

//***************************************************************************
void	CFaceStore::BuildOutQuadList()
{
/*
int		FaceCount=QuadFaceList.size();
		OutQuadList.resize(FaceCount);
		for (int i=0; i<FaceCount; i++)
		{
			CFace	&InFace=QuadFaceList[i];
			sQuad	&OutFace=OutQuadList[i];

			OutFace.P0=AddVtx(InFace.vtx[0]*Scale);
			OutFace.P1=AddVtx(InFace.vtx[1]*Scale);
			OutFace.P2=AddVtx(InFace.vtx[2]*Scale);
			OutFace.P3=AddVtx(InFace.vtx[3]*Scale);

// Materials and other shit
			SetupUV(InFace,OutFace);
		}
*/
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
int		CFaceStore::WriteTriList(FILE *File)
{
int		ListSize=OutTriList.size();
int		Pos=ftell(File);

		for (int i=0; i<ListSize; i++)
		{
			sTri	ThisTri=OutTriList[i];
			fwrite(&ThisTri,1,sizeof(sTri),File);
		}
		return(Pos);
}

//***************************************************************************
int		CFaceStore::WriteQuadList(FILE *File)
{
int		ListSize=OutQuadList.size();
int		Pos=ftell(File);

		for (int i=0; i<ListSize; i++)
		{
			sQuad	&ThisQuad=OutQuadList[i];
			fwrite(&ThisQuad,1,sizeof(sQuad),File);
		}
		return(Pos);
}

//***************************************************************************
int		CFaceStore::WriteVtxList(FILE *File,sVtx *Mul)
{
int		ListSize=OutVtxList.size();
int		Pos=ftell(File);

		for (int i=0; i<ListSize; i++)
		{
			sVtx	ThisVtx=OutVtxList[i];
			if (Mul)
			{
				ThisVtx.vx*=Mul->vx;
				ThisVtx.vy*=Mul->vy;
				ThisVtx.vz*=Mul->vz;
			}
			fwrite(&ThisVtx,1,sizeof(sVtx),File);
		}
		return(Pos);
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
//***************************************************************************
//***************************************************************************
//***************************************************************************
//***************************************************************************
#if	0	// quadding stuff
bool CFaceStore::CanConnect( int f0, int f1 )
{
	CFace &F0 = FaceList[f0];
	CFace &F1 = FaceList[f1];

	// check ID's match (used for nodes, weights)
	if (F0.ID!=F1.ID) return false;

	// check materials match
	if (F0.Mat != F1.Mat) return false;

	// check normals
	float dp = dotProduct( F0.Normal, F1.Normal );
	if (dp < 0.98f)
		return false;

	// check pnt connections
	bool found = false;
	int  chkp00 ;
	int  chkp01 ;
	int  chkp10 ;
	int  chkp11 ;
	for (int i=0; i<3 && !found; i++)
	{
		int npi = (i + 1)%3;
		for (int j=0; j<3 && !found; j++)
		{
			int npj = (j + 1)%3;
			if (F0.pts[i] == F1.pts[npj] && F0.pts[npi] == F1.pts[j])
			{
				chkp00 = i;
				chkp01 = npi;
				chkp10 = j;
				chkp11 = npj;
				found = true;
			}
		}
	}

	if (found)
	{
		// check uv
		return (uvaprox(F0.uvs[chkp00], F1.uvs[chkp11]) && uvaprox(F0.uvs[chkp01], F1.uvs[chkp10]));
	}

	return false;
}

//***************************************************************************
int CFaceStore::GetUnconnectedPoint(int f0, int f1, int &f0p0, int &f0p1 )
{
	CFace &F0 = FaceList[f0];
	CFace &F1 = FaceList[f1];

	for (int i=0; i<3; i++)
	{
		int npi = (i + 1)%3;
		for (int j=0; j<3; j++)
		{
			int npj = (j + 1)%3;
			if (F0.pts[i] == F1.pts[npj] && F0.pts[npi] == F1.pts[j])
			{
				int cp = (j+2) % 3;
				f0p0 = F0.pts[i];
				f0p1 = F0.pts[npi];
				return cp;
			}
		}
	}

	return -1;
}

//***************************************************************************
int CFaceStore::CountFacesAttached ( int f )
{
	int count = 0;
	int c = FaceList.size();

	for (int i=0; i<c; i++)
	{
		if (FaceList[i].avail && i!=f)
		{
			if (CanConnect(f, i))
			{
				count ++;
			}
		}
	}
	
	return count;
}

//***************************************************************************
void CFaceStore::FollowFace( int id, CFace &F )
{
	int minF = -1;
	int minC = 9999;
	int c = FaceList.size();
	for (int i=0; i<c; i++)
	{
		if (FaceList[i].avail && id != i && CanConnect(id, i))
		{
			int fec = CountFacesAttached( i );
			if (fec < minC)
			{
				minF = i;
				minC = fec;
			}
		}
	}

	CFace &nf = FaceList[minF];

	int ptc = F.pts.size();
	int v0, v1;
	int unc = GetUnconnectedPoint( id, minF, v0, v1);

	for (int j=0; j<ptc; j++) if (F.pts[j] == v1) break;

	
	if (j != (ptc-1))
	{
		int idx = (j+1)%ptc;
		vector<int>::iterator pb, pe, pm;
		vector<sUV>::iterator ub, ue, um;
		vector<Vector3>::iterator vb, ve, vm;
		pb = F.pts.begin();	pe = F.pts.end();
		ub = F.uvs.begin();	ue = F.uvs.end();
		vb = F.vtx.begin();	ve = F.vtx.end();

		pm = pb + idx;
		um = ub + idx;
		vm = vb + idx;

		rotate(pb, pm, pe);
		rotate(ub, um, ue);
		rotate(vb, vm, ve);
	}

	for (j=0; j<ptc; j++) if (F.pts[j] == v0)	break;

	if (j != (ptc-2))
	{
		int idx = (j+1)%ptc;
		vector<int>::iterator pb, pe, pm;
		vector<sUV>::iterator ub, ue, um;
		vector<Vector3>::iterator vb, ve, vm;
		pb = F.pts.begin();	pe = F.pts.end()-1;
		ub = F.uvs.begin();	ue = F.uvs.end()-1;
		vb = F.vtx.begin();	ve = F.vtx.end()-1;

		pm = pb + idx;
		um = ub + idx;
		vm = vb + idx;

		rotate(pb, pm, pe);
		rotate(ub, um, ue);
		rotate(vb, vm, ve);
	}

	F.pts.resize(ptc+1);
	F.vtx.resize(ptc+1);
	F.uvs.resize(ptc+1);

	F.pts[ptc] = nf.pts[unc];
	F.vtx[ptc] = nf.vtx[unc];
	F.uvs[ptc] = nf.uvs[unc];
	nf.avail = false;

	if (minC && MaxStrip>F.pts.size()) FollowFace( minF, F );
}

//***************************************************************************
bool	CFaceStore::GetFace( CFace &F )
{
int		minF = -1;
int		minC = 9999;
int		c = FaceList.size();

	for (int i=0; i<c; i++)
		{
		if (FaceList[i].avail)
			{
			int fec = CountFacesAttached( i );
			if (fec < minC)
				{
				minF = i;
				minC = fec;
				}
			}
		}

	if (minF == -1)
		return false;

	F.Mat = FaceList[minF].Mat;
	F.pts = FaceList[minF].pts;
	F.uvs = FaceList[minF].uvs;
	F.vtx = FaceList[minF].vtx;
	FaceList[minF].avail = false;
	F.Normal= FaceList[minF].Normal;
	F.ID= FaceList[minF].ID;

	if (minC && MaxStrip>F.pts.size()) FollowFace( minF, F );

	return true;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CFaceStore::QuadGetPnts(CFace &F,int *Join0,int *Join1,int *Pnt)
{
		if (!F.vis[0])
			{
			*Join0=	F.pts[0];
			*Join1=	F.pts[1];
			*Pnt=	2;
			}
		else
		if (!F.vis[1])
			{
			*Join0=	F.pts[1];
			*Join1=	F.pts[2];
			*Pnt=	0;
			}
		else
			{
			*Join0=	F.pts[2];
			*Join1=	F.pts[0];
			*Pnt=	1;
			}
}

//***************************************************************************
int		CFaceStore::QuadGetAttached(int FaceNo)
{
CFace	ThisFace=FaceList[FaceNo];
int		Vis=ThisFace.vis[0]+ThisFace.vis[1]+ThisFace.vis[2];
		if (Vis==7) return(0);	// Pure Tri
int		FaceCount=FaceList.size();
int		J00,J01,P0;
int		J10,J11,P1;

		QuadGetPnts(ThisFace,&J00,&J01,&P0);
	
		for (int Loop=FaceNo+1;Loop<FaceCount;Loop++)
			{
			CFace	&cFace=FaceList[Loop];
			if (cFace.avail)
				{
				if (ThisFace.Normal.x==cFace.Normal.x &&
					ThisFace.Normal.y==cFace.Normal.y &&
					ThisFace.Normal.z==cFace.Normal.z)
					{
					if (ThisFace.Mat==cFace.Mat && ThisFace.ID==cFace.ID)
						{
						QuadGetPnts(cFace,&J10,&J11,&P1);
						if ((J00==J10 && J01==J11) || (J00==J11 && J01==J10)) return(Loop);
						}
					}
				}
			}

	return(0);
}

//***************************************************************************
void CFaceStore::OrderPnts( CFace &F ,int unc)
{
int	idx;
	if (!F.vis[0]) idx=2;
	if (!F.vis[1]) idx=0;
	if (!F.vis[2]) idx=1;

	{
		vector<int>::iterator pb, pe, pm;
		vector<sUV>::iterator ub, ue, um;
		vector<Vector3>::iterator vb, ve, vm;
		pb = F.pts.begin();	pe = F.pts.end();
		ub = F.uvs.begin();	ue = F.uvs.end();
		vb = F.vtx.begin();	ve = F.vtx.end();

		pm = pb + idx;
		um = ub + idx;
		vm = vb + idx;

		rotate(pb, pm, pe);
		rotate(ub, um, ue);
		rotate(vb, vm, ve);
	}

}

//***************************************************************************
void	CFaceStore::Quad(vector<CFace> &TriList,vector<CFace> &QuadList)
{
int	FaceCount=FaceList.size();

	if (MaxStrip==4)
	{
//vector<CFace> ThisTriList;
	for (int Loop=0;Loop<FaceCount;Loop++)
	{
		if (FaceList[Loop].avail)
		{
			int	Att=QuadGetAttached(Loop);
			if (Att)
			{
				CFace &F=FaceList[Loop];
				CFace &NF=FaceList[Att];
				int	J0,J1,P;
				QuadGetPnts(NF,&J0,&J1,&P);
				FaceList[Loop].avail=FaceList[Att].avail=false;
				OrderPnts(F,P);
				F.pts.push_back(NF.pts[P]);
				F.vtx.push_back(NF.vtx[P]);
				F.uvs.push_back(NF.uvs[P]);
				QuadList.push_back(F);
			}
		}
	}

// Strip remaining tris
//	for (Loop=0;Loop<FaceCount;Loop++)
//		{
//		if (FaceList[Loop].avail) ThisTriList.push_back(FaceList[Loop]);
//		}

		bool strips = true;
		while (strips)
			{
			CFace f;
			strips = GetFace( f );
			if (strips)
				{
				if (f.pts.size() == 3)
					TriList.push_back(f); 
				else
					QuadList.push_back(f); 
				}
			}
//		printf("Quaded %i extra\n",ThisTriList.size()-TriList.size());
		}
	else
		{	// No quadding, copy direct
		for (int Loop=0;Loop<FaceCount;Loop++) TriList.push_back(FaceList[Loop]);
		}
}
#endif
