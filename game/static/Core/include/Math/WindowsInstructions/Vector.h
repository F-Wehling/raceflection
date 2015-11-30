#pragma once

#include <Math/WindowsInstructions/__instructions.h>

BEGINNAMESPACE

//
/// Vector Arithmetics
DX_NAMEALIAS(VectorAbs);
DX_NAMEALIAS(VectorAdd);
DX_NAMEALIAS(VectorAddAngles);
DX_NAMEALIAS(VectorCeiling);
DX_NAMEALIAS(VectorClamp);
DX_NAMEALIAS(VectorDivide);
DX_NAMEALIAS(VectorFloor);
DX_NAMEALIAS(VectorIsInfinite);
DX_NAMEALIAS(VectorIsNaN);
DX_NAMEALIAS(VectorMax);
DX_NAMEALIAS(VectorMin);
DX_NAMEALIAS(VectorMod);
DX_NAMEALIAS(VectorModAngles);
DX_NAMEALIAS(VectorMultiply);
DX_NAMEALIAS(VectorMultiplyAdd);
DX_NAMEALIAS(VectorNegate);
DX_NAMEALIAS(VectorNegativeMultiplySubtract);
DX_NAMEALIAS(VectorPow);
DX_NAMEALIAS(VectorReciprocal);
DX_NAMEALIAS(VectorReciprocalEst);
DX_NAMEALIAS(VectorReciprocalSqrt);
DX_NAMEALIAS(VectorReciprocalSqrtEst);
DX_NAMEALIAS(VectorRound);
DX_NAMEALIAS(VectorSaturate);
DX_NAMEALIAS(VectorScale);
DX_NAMEALIAS(VectorSqrt);
DX_NAMEALIAS(VectorSqrtEst);
DX_NAMEALIAS(VectorSubtract);
DX_NAMEALIAS(VectorSubtractAngles);
DX_NAMEALIAS(VectorTruncate);

//
/// Vector bitwise
DX_NAMEALIAS(VectorAndCInt);
DX_NAMEALIAS(VectorAndInt);
DX_NAMEALIAS(VectorNorInt);
DX_NAMEALIAS(VectorNotEqual);
DX_NAMEALIAS(VectorNotEqualInt);
DX_NAMEALIAS(VectorOrInt);
DX_NAMEALIAS(VectorXorInt);

//
/// Vector Compare
DX_NAMEALIAS(VectorEqual);
DX_NAMEALIAS(VectorEqualInt);
DX_NAMEALIAS(VectorEqualIntR);
DX_NAMEALIAS(VectorEqualR);
DX_NAMEALIAS(VectorGreater);
DX_NAMEALIAS(VectorGreaterOrEqual);
DX_NAMEALIAS(VectorGreaterOrEqualR);
DX_NAMEALIAS(VectorGreaterR);
DX_NAMEALIAS(VectorLess);
DX_NAMEALIAS(VectorLessOrEqual);
DX_NAMEALIAS(VectorNearEqual);

//
/// Component wise

//the following functions are defined as template and as normal-function.
//to allow the compiler to distinguish between them:
typedef DirectX::XMVECTOR(XM_CALLCONV*_tVectorInsert)(DirectX::FXMVECTOR, DirectX::FXMVECTOR, uint32_t,uint32_t, uint32_t, uint32_t, uint32_t);
typedef DirectX::XMVECTOR(XM_CALLCONV*_tVectorPermute)(DirectX::FXMVECTOR, DirectX::FXMVECTOR, uint32_t, uint32_t, uint32_t, uint32_t);
typedef DirectX::XMVECTOR(XM_CALLCONV*_tVectorRotateLeft)(DirectX::FXMVECTOR, uint32_t);
typedef DirectX::XMVECTOR(XM_CALLCONV*_tVectorRotateRight)(DirectX::FXMVECTOR, uint32_t);
typedef DirectX::XMVECTOR(XM_CALLCONV*_tVectorShiftLeft)(DirectX::FXMVECTOR, DirectX::FXMVECTOR, uint32_t);
typedef DirectX::XMVECTOR(XM_CALLCONV*_tVectorSwizzle)(DirectX::FXMVECTOR, uint32_t, uint32_t, uint32_t, uint32_t);

extern const _tVectorInsert& VectorInsert;
extern const _tVectorPermute& VectorPermute;
DX_NAMEALIAS(VectorMergeXY);
DX_NAMEALIAS(VectorMergeZW);
extern const _tVectorRotateLeft& VectorRotateLeft;
extern const _tVectorRotateRight& VectorRotateRight;
DX_NAMEALIAS(VectorSelect);
DX_NAMEALIAS(VectorSelectControl);
extern const _tVectorShiftLeft& VectorShiftLeft;
DX_NAMEALIAS(VectorSplatW);
DX_NAMEALIAS(VectorSplatX);
DX_NAMEALIAS(VectorSplatY);
DX_NAMEALIAS(VectorSplatZ);
extern const _tVectorSwizzle& VectorSwizzle;
//DX_NAMEALIAS(VectorSwizzle);

template<uint32_t Elements> FORCE_INLINE DirectX::XMVECTOR TVectorShiftLeft(DirectX::FXMVECTOR V1, DirectX::FXMVECTOR V2) { DirectX::XMVectorShiftLeft<Elements>(V1, V2); }
template<uint32_t Elements> FORCE_INLINE DirectX::XMVECTOR TVectorRotateLeft(DirectX::FXMVECTOR V) { return DirectX::XMVectorRotateLeft<Elements>(V); }
template<uint32_t Elements> FORCE_INLINE DirectX::XMVECTOR TVectorRotateRight(DirectX::FXMVECTOR V) { return DirectX::XMVectorRotateRight<Elements>(V); }
template<uint32_t VSLeftRotateElements, uint32_t Select0, uint32_t Select1, uint32_t Select2, uint32_t Select3> 
FORCE_INLINE DirectX::XMVECTOR TVectorInsert(DirectX::FXMVECTOR VD, DirectX::FXMVECTOR VS) { return DirectX::XMVectorInsert<VSLeftRotateElements, Select0, Select1, Select2, Select3>(VD, VS); }
template<uint32_t PermuteX, uint32_t PermuteY, uint32_t PermuteZ, uint32_t PermuteW> FORCE_INLINE DirectX::XMVECTOR TVectorPermute(DirectX::FXMVECTOR V1, DirectX::FXMVECTOR V2) {return DirectX::XMVectorPermute<PermuteX, PermuteY, PermuteZ, PermuteW>(V1, V2);}
template<uint32_t SwizzleX, uint32_t SwizzleY, uint32_t SwizzleZ, uint32_t SwizzleW> FORCE_INLINE DirectX::XMVECTOR TVectorSwizzle(DirectX::FXMVECTOR V) { return DirectX::XMVectorSwizzle<SwizzleX, SwizzleY, SwizzleZ, SwizzleW>(V); }

//
/// Geometric functions
DX_NAMEALIAS(VectorBaryCentric);
DX_NAMEALIAS(VectorBaryCentricV);
DX_NAMEALIAS(VectorCatmullRom);
DX_NAMEALIAS(VectorCatmullRomV);
DX_NAMEALIAS(VectorHermite);
DX_NAMEALIAS(VectorHermiteV);
DX_NAMEALIAS(VectorInBounds);
DX_NAMEALIAS(VectorInBoundsR);
DX_NAMEALIAS(VectorLerp);
DX_NAMEALIAS(VectorLerpV);

//
/// Vector Initialize
DX_NAMEALIAS(VectorFalseInt);
DX_NAMEALIAS(VectorReplicate);
DX_NAMEALIAS(VectorReplicateInt);
DX_NAMEALIAS(VectorReplicateIntPtr);
DX_NAMEALIAS(VectorReplicatePtr);
DX_NAMEALIAS(VectorSet);
DX_NAMEALIAS(VectorSetBinaryConstant);
DX_NAMEALIAS(VectorSetInt);
DX_NAMEALIAS(VectorSplatConstant);
DX_NAMEALIAS(VectorSplatConstantInt);
DX_NAMEALIAS(VectorSplatEpsilon);
DX_NAMEALIAS(VectorSplatInfinity);
DX_NAMEALIAS(VectorSplatOne);
DX_NAMEALIAS(VectorSplatQNaN);
DX_NAMEALIAS(VectorSplatSignMask);
DX_NAMEALIAS(VectorTrueInt);
DX_NAMEALIAS(VectorZero);

//
/// transcendental vector function
DX_NAMEALIAS(VectorACos);
DX_NAMEALIAS(VectorACosEst);
DX_NAMEALIAS(VectorASin);
DX_NAMEALIAS(VectorASinEst);
DX_NAMEALIAS(VectorATan);
DX_NAMEALIAS(VectorATan2);
DX_NAMEALIAS(VectorATan2Est);
DX_NAMEALIAS(VectorATanEst);
DX_NAMEALIAS(VectorCos);
DX_NAMEALIAS(VectorCosEst);
DX_NAMEALIAS(VectorCosH);
DX_NAMEALIAS(VectorExp);
DX_NAMEALIAS(VectorExp2);
DX_NAMEALIAS(VectorExpE);
DX_NAMEALIAS(VectorLog);
DX_NAMEALIAS(VectorLog2);
DX_NAMEALIAS(VectorLogE);
DX_NAMEALIAS(VectorSin);
DX_NAMEALIAS(VectorSinCos);
DX_NAMEALIAS(VectorSinCosEst);
DX_NAMEALIAS(VectorSinEst);
DX_NAMEALIAS(VectorSinH);
DX_NAMEALIAS(VectorTan);
DX_NAMEALIAS(VectorTanEst);
DX_NAMEALIAS(VectorTanH);

//
///Specialized Versions for 2D Vector

//compare
DX_NAMEALIAS(Vector2Equal);
DX_NAMEALIAS(Vector2EqualInt);
DX_NAMEALIAS(Vector2EqualIntR);
DX_NAMEALIAS(Vector2EqualR);
DX_NAMEALIAS(Vector2Greater);
DX_NAMEALIAS(Vector2GreaterOrEqual);
DX_NAMEALIAS(Vector2GreaterOrEqualR);
DX_NAMEALIAS(Vector2GreaterR);
DX_NAMEALIAS(Vector2IsInfinite);
DX_NAMEALIAS(Vector2IsNaN);
DX_NAMEALIAS(Vector2Less);
DX_NAMEALIAS(Vector2LessOrEqual);
DX_NAMEALIAS(Vector2NearEqual);
DX_NAMEALIAS(Vector2NotEqual);
DX_NAMEALIAS(Vector2NotEqualInt);

//geometric
DX_NAMEALIAS(Vector2AngleBetweenNormals);
DX_NAMEALIAS(Vector2AngleBetweenNormalsEst);
DX_NAMEALIAS(Vector2AngleBetweenVectors);
DX_NAMEALIAS(Vector2ClampLength);
DX_NAMEALIAS(Vector2ClampLengthV);
DX_NAMEALIAS(Vector2Cross);
DX_NAMEALIAS(Vector2Dot);
DX_NAMEALIAS(Vector2InBounds);
DX_NAMEALIAS(Vector2IntersectLine);
DX_NAMEALIAS(Vector2Length);
DX_NAMEALIAS(Vector2LengthEst);
DX_NAMEALIAS(Vector2LengthSq);
DX_NAMEALIAS(Vector2LinePointDistance);
DX_NAMEALIAS(Vector2Normalize);
DX_NAMEALIAS(Vector2NormalizeEst);
DX_NAMEALIAS(Vector2Orthogonal);
DX_NAMEALIAS(Vector2ReciprocalLength);
DX_NAMEALIAS(Vector2ReciprocalLengthEst);
DX_NAMEALIAS(Vector2Reflect);
DX_NAMEALIAS(Vector2Refract);
DX_NAMEALIAS(Vector2RefractV);

//vector-matrix transformation
DX_NAMEALIAS(Vector2Transform);
DX_NAMEALIAS(Vector2TransformCoord);
DX_NAMEALIAS(Vector2TransformCoordStream);
DX_NAMEALIAS(Vector2TransformNormal);
DX_NAMEALIAS(Vector2TransformNormalStream);
DX_NAMEALIAS(Vector2TransformStream);

//
///Specialized Versions for 3D Vector

//compare
DX_NAMEALIAS(Vector3Equal);
DX_NAMEALIAS(Vector3EqualInt);
DX_NAMEALIAS(Vector3EqualIntR);
DX_NAMEALIAS(Vector3EqualR);
DX_NAMEALIAS(Vector3Greater);
DX_NAMEALIAS(Vector3GreaterOrEqual);
DX_NAMEALIAS(Vector3GreaterOrEqualR);
DX_NAMEALIAS(Vector3GreaterR);
DX_NAMEALIAS(Vector3IsInfinite);
DX_NAMEALIAS(Vector3IsNaN);
DX_NAMEALIAS(Vector3Less);
DX_NAMEALIAS(Vector3LessOrEqual);
DX_NAMEALIAS(Vector3NearEqual);
DX_NAMEALIAS(Vector3NotEqual);
DX_NAMEALIAS(Vector3NotEqualInt);

//geometric
DX_NAMEALIAS(Vector3AngleBetweenNormals);
DX_NAMEALIAS(Vector3AngleBetweenNormalsEst);
DX_NAMEALIAS(Vector3AngleBetweenVectors);
DX_NAMEALIAS(Vector3ClampLength);
DX_NAMEALIAS(Vector3ClampLengthV);
DX_NAMEALIAS(Vector3ComponentsFromNormal);
DX_NAMEALIAS(Vector3Cross);
DX_NAMEALIAS(Vector3Dot);
DX_NAMEALIAS(Vector3InBounds);
DX_NAMEALIAS(Vector3Length);
DX_NAMEALIAS(Vector3LengthEst);
DX_NAMEALIAS(Vector3LengthSq);
DX_NAMEALIAS(Vector3LinePointDistance);
DX_NAMEALIAS(Vector3Normalize);
DX_NAMEALIAS(Vector3NormalizeEst);
DX_NAMEALIAS(Vector3Orthogonal);
DX_NAMEALIAS(Vector3ReciprocalLength);
DX_NAMEALIAS(Vector3ReciprocalLengthEst);
DX_NAMEALIAS(Vector3Reflect);
DX_NAMEALIAS(Vector3Refract);
DX_NAMEALIAS(Vector3RefractV);

//vector-matrix transformation
DX_NAMEALIAS(Vector3InverseRotate);
DX_NAMEALIAS(Vector3Project);
DX_NAMEALIAS(Vector3ProjectStream);
DX_NAMEALIAS(Vector3Rotate);
DX_NAMEALIAS(Vector3Transform);
DX_NAMEALIAS(Vector3TransformCoord);
DX_NAMEALIAS(Vector3TransformCoordStream);
DX_NAMEALIAS(Vector3TransformNormal);
DX_NAMEALIAS(Vector3TransformNormalStream);
DX_NAMEALIAS(Vector3TransformStream);
DX_NAMEALIAS(Vector3Unproject);
DX_NAMEALIAS(Vector3UnprojectStream);

//
///Specialized Versions for 4D Vector

//compare
DX_NAMEALIAS(Vector4Equal);
DX_NAMEALIAS(Vector4EqualInt);
DX_NAMEALIAS(Vector4EqualIntR);
DX_NAMEALIAS(Vector4EqualR);
DX_NAMEALIAS(Vector4Greater);
DX_NAMEALIAS(Vector4GreaterOrEqual);
DX_NAMEALIAS(Vector4GreaterOrEqualR);
DX_NAMEALIAS(Vector4GreaterR);
DX_NAMEALIAS(Vector4IsInfinite);
DX_NAMEALIAS(Vector4IsNaN);
DX_NAMEALIAS(Vector4Less);
DX_NAMEALIAS(Vector4LessOrEqual);
DX_NAMEALIAS(Vector4NearEqual);
DX_NAMEALIAS(Vector4NotEqual);
DX_NAMEALIAS(Vector4NotEqualInt);

//geometric
DX_NAMEALIAS(Vector4AngleBetweenNormals);
DX_NAMEALIAS(Vector4AngleBetweenNormalsEst);
DX_NAMEALIAS(Vector4AngleBetweenVectors);
DX_NAMEALIAS(Vector4ClampLength);
DX_NAMEALIAS(Vector4ClampLengthV);
DX_NAMEALIAS(Vector4Cross);
DX_NAMEALIAS(Vector4Dot);
DX_NAMEALIAS(Vector4InBounds);
DX_NAMEALIAS(Vector4Length);
DX_NAMEALIAS(Vector4LengthEst);
DX_NAMEALIAS(Vector4LengthSq);
DX_NAMEALIAS(Vector4Normalize);
DX_NAMEALIAS(Vector4NormalizeEst);
DX_NAMEALIAS(Vector4Orthogonal);
DX_NAMEALIAS(Vector4ReciprocalLength);
DX_NAMEALIAS(Vector4ReciprocalLengthEst);
DX_NAMEALIAS(Vector4Reflect);
DX_NAMEALIAS(Vector4Refract);
DX_NAMEALIAS(Vector4RefractV);

//Vector-matrix transform#
DX_NAMEALIAS(Vector4Transform);
DX_NAMEALIAS(Vector4TransformStream);

ENDNAMESPACE