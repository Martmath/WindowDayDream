#pragma once
#include <limits>
#include "Triple.h"
#include <math.h>

/*math based at
1)https://habr.com/ru/post/255005/  -ru  https://weekly-geekly.github.io/articles/255005/index.html -en VB use
2)https://github.com/Zottelx22/SBE/blob/master/Engine/Math/Quaternion.cpp

todo convert from C-style to C++-style
*/

constexpr double dPI = 3.141592653589793238462643383279502884;
constexpr double RAD2DEG = 180.0 / dPI;

using TVector = TripleXYZ<double>;
using TPoint = TripleXYZ<double>;

struct TAxis
{
	TPoint p;
	TVector	XA;
	TVector	YA;
	TVector	ZA;
};// static ViewAxis();

struct PAxis
{
	TPoint p;
	TPoint XP;
	TPoint YP;
	TPoint ZP;
};

struct TQuat
{
	DOUBLE WQ;
	DOUBLE XQ;
	DOUBLE YQ;
	DOUBLE ZQ;
};

struct TEulerPxRyYz
{
	DOUBLE PitchX;
	DOUBLE RollY;
	DOUBLE YawZ;
};

struct Math3D
{
	static constexpr void VProduct(TVector& res, const TVector& a, const TVector& b)
	{
		res.x = a.y * b.z - a.z * b.y;
		res.y = a.z * b.x - a.x * b.z;
		res.z = a.x * b.y - a.y * b.x;
	};

	static TVector VProduct(const TVector& a, const TVector& b) {
		TVector res;
		VProduct(res, a, b);
		return res;
	};

	static constexpr void TransformAxis(TAxis& res, const TAxis& from)
	{
		res.XA.x = from.XA.x;
		res.XA.y = from.YA.x;
		res.XA.z = from.ZA.x;

		res.YA.x = from.XA.y;
		res.YA.y = from.YA.y;
		res.YA.z = from.ZA.y;

		res.ZA.x = from.XA.z;
		res.ZA.y = from.YA.z;
		res.ZA.z = from.ZA.z;

		res.p.x = -(res.XA.x * from.p.x + res.YA.x * from.p.y + res.ZA.x * from.p.z);
		res.p.y = -(res.XA.y * from.p.x + res.YA.y * from.p.y + res.ZA.y * from.p.z);
		res.p.z = -(res.XA.z * from.p.x + res.YA.z * from.p.y + res.ZA.z * from.p.z);

	};

	static TAxis TransformAxis(const TAxis& from)
	{
		TAxis res;
		TransformAxis(res, from);
		return res;
	};

	//https://stackoverflow.com/questions/8622256/in-c11-is-sqrt-defined-as-constexpr
	static DOUBLE constexpr sqrtNewtonRaphson(DOUBLE x, DOUBLE curr, DOUBLE prev)
	{
		return curr == prev
			? curr
			: sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
	};

	static DOUBLE constexpr csqrt(DOUBLE x)
	{
		return x >= 0 && x < std::numeric_limits<DOUBLE>::infinity()
			? sqrtNewtonRaphson(x, x, 0)
			: std::numeric_limits<DOUBLE>::quiet_NaN();
	};

	static  DOUBLE Normal(TVector& inVector)
	{
		DOUBLE L = sqrt(inVector.x * inVector.x + inVector.y * inVector.y + inVector.z * inVector.z);
		inVector.x = inVector.x / L;
		inVector.y = inVector.y / L;
		inVector.z = inVector.z / L;
		return L;
	};

	static void Normal(TAxis& inAxis)
	{
		Normal(inAxis.XA);
		Normal(inAxis.YA);
		Normal(inAxis.ZA);
	};

	static  void  GPAxis(PAxis& res, const TAxis& from)
	{
		res.p = from.p;
		res.XP.x = from.XA.x + from.p.x;
		res.XP.y = from.XA.y + from.p.y;
		res.XP.z = from.XA.z + from.p.z;

		res.YP.x = from.YA.x + from.p.x;
		res.YP.y = from.YA.y + from.p.y;
		res.YP.z = from.YA.z + from.p.z;

		res.ZP.x = from.ZA.x + from.p.x;
		res.ZP.y = from.ZA.y + from.p.y;
		res.ZP.z = from.ZA.z + from.p.z;
	};

	static PAxis GPAxisRet(const TAxis& a)
	{
		PAxis res;
		GPAxis(res, a);
		return res;
	};

	template<bool Normalize = false>
	static  void  GTAxis(TAxis& res, const PAxis& from)
	{
		res.p = from.p;
		res.XA.x = from.XP.x - from.p.x;
		res.XA.y = from.XP.y - from.p.y;
		res.XA.z = from.XP.z - from.p.z;

		res.YA.x = from.YP.x - from.p.x;
		res.YA.y = from.YP.y - from.p.y;
		res.YA.z = from.YP.z - from.p.z;

		res.ZA.x = from.ZP.x - from.p.x;
		res.ZA.y = from.ZP.y - from.p.y;
		res.ZA.z = from.ZP.z - from.p.z;
		if constexpr (Normalize) Normal(res);

	};

	template<bool Normalize = true>
	static TAxis GTAxisRet(const PAxis& a)
	{
		TAxis res;
		GTAxis<Normalize>(res, a);
		return res;
	};

	static constexpr void  VectorToOtherAxis(TVector& res, const TAxis& a, const TVector& p)
	{
		res.x = a.XA.x * p.x + a.YA.x * p.y + a.ZA.x * p.z;
		res.y = a.XA.y * p.x + a.YA.y * p.y + a.ZA.y * p.z;
		res.z = a.XA.z * p.x + a.YA.z * p.y + a.ZA.z * p.z;
	};

	//a - axis in coord1, p - vector in coord1, result -vector p in 'a' coordinates
	static TVector VectorToOtherAxisRet(const TAxis& a, const TVector& p)
	{
		TVector res;
		VectorToOtherAxis(res, a, p);
		return res;
	};

	template<bool Normalize = true>
	static void createQuat(TQuat& q, TVector& rotateVector, DOUBLE rotateAngle)
	{
		if constexpr (Normalize) Normal(rotateVector);
		q.WQ = cos(rotateAngle / 2);
		q.XQ = rotateVector.x * sin(rotateAngle / 2);
		q.YQ = rotateVector.y * sin(rotateAngle / 2);
		q.ZQ = rotateVector.z * sin(rotateAngle / 2);
	};

	template<bool Normalize = false>
	static TQuat createQuatRet(TVector& rotate_vector, DOUBLE rotate_angle)
	{
		TQuat res;
		createQuat<Normalize>(res, rotate_vector, rotate_angle);
		return res;
	};

	static  DOUBLE Deg2Rad(DOUBLE deg)
	{
		return deg / RAD2DEG;
	}

	static DOUBLE Rad2Deg(DOUBLE rad)
	{

		return RAD2DEG * rad;
	}

	static void quatNormalize(TQuat& res, const TQuat& q)
	{
		DOUBLE n = sqrt(q.WQ * q.WQ + q.XQ * q.XQ + q.YQ * q.YQ + q.ZQ * q.ZQ);
		res.WQ = q.WQ / n;// ' len
		res.XQ = q.XQ / n;// ' x
		res.YQ = q.YQ / n;// ' y
		res.ZQ = q.ZQ / n;// ' z	
	}
	static TQuat& quatNormalizeSelf(TQuat& q)
	{
		DOUBLE n = sqrt(q.WQ * q.WQ + q.XQ * q.XQ + q.YQ * q.YQ + q.ZQ * q.ZQ);
		q.WQ = q.WQ / n;// ' l
		q.XQ = q.XQ / n;// ' x
		q.YQ = q.YQ / n;// ' y
		q.ZQ = q.ZQ / n;// ' z	
		return q;
	}
	static TQuat quatNormalizeRet(const TQuat& q)
	{
		TQuat res;
		quatNormalize(res, q);
		return res;
	}

	static void quatInvert(TQuat& res, const TQuat& q)
	{
		res.WQ = q.WQ;
		res.XQ = -q.XQ;
		res.YQ = -q.YQ;
		res.ZQ = -q.ZQ;
		quatNormalizeSelf(res);
	}

	static TQuat& quatInvertSelf(TQuat& q)
	{
		q.XQ = -q.XQ;
		q.YQ = -q.YQ;
		q.ZQ = -q.ZQ;
		quatNormalizeSelf(q);
		return q;
	}

	static TQuat quatInvertRet(const TQuat& q)
	{
		TQuat res;
		quatInvert(res, q);
		return res;
	}

	static void quatMulQuat(TQuat& res, const TQuat& a, const TQuat& b)
	{
		res.WQ = a.WQ * b.WQ - a.XQ * b.XQ - a.YQ * b.YQ - a.ZQ * b.ZQ;
		res.XQ = a.WQ * b.XQ + a.XQ * b.WQ + a.YQ * b.ZQ - a.ZQ * b.YQ;
		res.YQ = a.WQ * b.YQ - a.XQ * b.ZQ + a.YQ * b.WQ + a.ZQ * b.XQ;
		res.ZQ = a.WQ * b.ZQ + a.XQ * b.YQ - a.YQ * b.XQ + a.ZQ * b.WQ;
	};

	static TQuat quatMulQuatRet(const TQuat& a, const TQuat& b)
	{
		TQuat res;
		quatMulQuat(res, a, b);
		return res;
	};

	static void quatMulVector(TQuat& res, const TQuat& a, const TVector& b)
	{
		res.WQ = -a.XQ * b.x - a.YQ * b.y - a.ZQ * b.z;
		res.XQ = a.WQ * b.x + a.YQ * b.z - a.ZQ * b.y;
		res.YQ = a.WQ * b.y - a.XQ * b.z + a.ZQ * b.x;
		res.ZQ = a.WQ * b.z + a.XQ * b.y - a.YQ * b.x;
	};

	static TQuat quatMulVectorRet(const TQuat& a, const TVector& b)
	{
		TQuat res;
		quatMulVector(res, a, b);
		return res;
	};


	static void quatTransformVector(TVector& resVector, const TQuat& q, const TVector& v)
	{
		TQuat T;
		T = quatMulVectorRet(q, v);
		T = quatMulQuatRet(T, quatInvertRet(q));
		resVector.x = T.XQ;
		resVector.y = T.YQ;
		resVector.z = T.ZQ;
	};

	static TVector quatTransformVectorRet(const TQuat& q, const TVector& v)
	{
		TVector res;
		quatTransformVector(res, q, v);
		return res;
	};
	static void quatTransformPAxis(PAxis& res, const TQuat& q, const PAxis& a)
	{
		res.p = quatTransformVectorRet(q, a.p);
		res.XP = quatTransformVectorRet(q, a.XP);
		res.YP = quatTransformVectorRet(q, a.YP);
		res.ZP = quatTransformVectorRet(q, a.ZP);
	}

	static PAxis quatTransformPAxisRet(const TQuat& q, const PAxis& a)
	{
		PAxis res;
		quatTransformPAxis(res, q, a);
		return res;
	};

	static void toAxisAngle(const TQuat& q, TVector& axis, DOUBLE& angle)
	{
		TQuat tempThis = quatNormalizeRet(q);

		angle = 2 * acos(tempThis.WQ);

		DOUBLE s = sqrt(1 - tempThis.WQ * tempThis.WQ);
		if (s < 0.0001)
		{
			axis.x = tempThis.XQ;
			axis.y = tempThis.YQ;
			axis.z = tempThis.ZQ;
		}
		else
		{
			axis.x = tempThis.XQ / s;
			axis.y = tempThis.YQ / s;
			axis.z = tempThis.ZQ / s;
		}
	}

	static TQuat quatFromAnglesRad(DOUBLE pitchX, DOUBLE rollY, DOUBLE yawZ)
	{
		TQuat qz;
		TQuat qy;
		TQuat qx;
		TQuat q_tmp;
		qz.XQ = 0;
		qz.YQ = 0;
		qz.ZQ = sin(yawZ / 2);
		qz.WQ = cos(yawZ / 2);

		qy.XQ = 0;
		qy.YQ = sin(rollY / 2);
		qy.ZQ = 0;
		qy.WQ = cos(rollY / 2);

		qx.XQ = sin(pitchX / 2);
		qx.YQ = 0;
		qx.ZQ = 0;
		qx.WQ = cos(pitchX / 2);

		q_tmp = quatMulQuatRet(qz, qy);
		return quatMulQuatRet(q_tmp, qx);
	}

};


/*'======================================
ViewAxis.YA.x = -0.877896
ViewAxis.YA.y = 0.128826
ViewAxis.YA.z = 0.461197

ViewAxis.ZA.x = 0
ViewAxis.ZA.y = -0.963131
ViewAxis.ZA.z = 0.269032
ViewAxis.XA = Math.VpV(ViewAxis.YA, ViewAxis.ZA)

ViewAxis.p.x = 13.408
ViewAxis.p.y = 10.466
ViewAxis.p.z = -29.44

BaseAxis = TM(ViewAxis)


Dim vax1 As TAxis
vax1 = TM(BaseAxis)


Dim ppa As PAxis
ppa = GPAxis(BaseAxis)
Dim ppa1 As PAxis


Dim VV As TVector 'in ax2
VV = TVectorC(-0.341276, -0.64024, 0.688203)

Dim VV2 As TVector
VV2 = TP3(BaseAxis, VV)
Dim VV1 As TVector
VV1 = TVectorC(0.267261, 0.534522, 0.801784)

Dim q As TQuat
q = create_quat(VV1, Deg2Rad(45))


ppa1 = KK(q, ppa)

Dim tta As TAxis
tta = GTAxis(ppa1)

Dim vax As TAxis
vax = TM(tta)


Dim YYz As TVector
YYz = quat_transform_vector(q, ToV(BaseAxis.p))


'-7.46mm -2.269mm -5.81mm
'6.979mm 27.577mm  18.622mm
Exit Sub
Dim vDirection(2)
vDirection(0) = VV.x
vDirection(1) = VV.y
vDirection(2) = VV.z
varViewer.Rotate vDirection, 45
ActViewer.Update

Dim ViewAxis1 As TAxis

Set ActDoc = CATIA.ActiveDocument
Set ActWin = CATIA.ActiveWindow

Set ActViewer = ActWin.ActiveViewer
Set varViewer = ActViewer

Set ActViewpoint = ActViewer.Viewpoint3D
Set varViewpoint = ActViewpoint

varViewpoint.GetUpDirection DirectionUpZ
varViewpoint.GetSightDirection DirectionSightY

varViewpoint.GetOrigin PointOrigin
Focus = ActViewpoint.FocusDistance

For I = 0 To 2
PointRotateOrigin(I) = PointOrigin(I) + Focus * (DirectionSightY(I)) ' - StdSightDirection(I))
Next
'======================================
ViewAxis1.YA.x = DirectionSightY(0)
ViewAxis1.YA.y = DirectionSightY(1)
ViewAxis1.YA.z = DirectionSightY(2)

ViewAxis1.ZA.x = DirectionUpZ(0)
ViewAxis1.ZA.y = DirectionUpZ(1)
ViewAxis1.ZA.z = DirectionUpZ(2)
ViewAxis1.XA = Math.VpV(ViewAxis1.YA, ViewAxis1.ZA)

ViewAxis1.p.x = PointRotateOrigin(0)
ViewAxis1.p.y = PointRotateOrigin(1)
ViewAxis1.p.z = PointRotateOrigin(2)
End Sub

*/






















class mCatia
{
};

