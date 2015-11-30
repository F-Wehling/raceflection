#if OS_WINDOWS

#include <Math/WindowsInstructions/Instructions.h>

BEGINNAMESPACE

//
/// Vector Arithmetics
DX_IMPL_NAMEALIAS(VectorAbs);
DX_IMPL_NAMEALIAS(VectorAdd);
DX_IMPL_NAMEALIAS(VectorAddAngles);
DX_IMPL_NAMEALIAS(VectorCeiling);
DX_IMPL_NAMEALIAS(VectorClamp);
DX_IMPL_NAMEALIAS(VectorDivide);
DX_IMPL_NAMEALIAS(VectorFloor);
DX_IMPL_NAMEALIAS(VectorIsInfinite);
DX_IMPL_NAMEALIAS(VectorIsNaN);
DX_IMPL_NAMEALIAS(VectorMax);
DX_IMPL_NAMEALIAS(VectorMin);
DX_IMPL_NAMEALIAS(VectorMod);
DX_IMPL_NAMEALIAS(VectorModAngles);
DX_IMPL_NAMEALIAS(VectorMultiply);
DX_IMPL_NAMEALIAS(VectorMultiplyAdd);
DX_IMPL_NAMEALIAS(VectorNegate);
DX_IMPL_NAMEALIAS(VectorNegativeMultiplySubtract);
DX_IMPL_NAMEALIAS(VectorPow);
DX_IMPL_NAMEALIAS(VectorReciprocal);
DX_IMPL_NAMEALIAS(VectorReciprocalEst);
DX_IMPL_NAMEALIAS(VectorReciprocalSqrt);
DX_IMPL_NAMEALIAS(VectorReciprocalSqrtEst);
DX_IMPL_NAMEALIAS(VectorRound);
DX_IMPL_NAMEALIAS(VectorSaturate);
DX_IMPL_NAMEALIAS(VectorScale);
DX_IMPL_NAMEALIAS(VectorSqrt);
DX_IMPL_NAMEALIAS(VectorSqrtEst);
DX_IMPL_NAMEALIAS(VectorSubtract);
DX_IMPL_NAMEALIAS(VectorSubtractAngles);
DX_IMPL_NAMEALIAS(VectorTruncate);

//
/// Vector bitwise
DX_IMPL_NAMEALIAS(VectorAndCInt);
DX_IMPL_NAMEALIAS(VectorAndInt);
DX_IMPL_NAMEALIAS(VectorNorInt);
DX_IMPL_NAMEALIAS(VectorNotEqual);
DX_IMPL_NAMEALIAS(VectorNotEqualInt);
DX_IMPL_NAMEALIAS(VectorOrInt);
DX_IMPL_NAMEALIAS(VectorXorInt);

//
/// Vector Compare
DX_IMPL_NAMEALIAS(VectorEqual);
DX_IMPL_NAMEALIAS(VectorEqualInt);
DX_IMPL_NAMEALIAS(VectorEqualIntR);
DX_IMPL_NAMEALIAS(VectorEqualR);
DX_IMPL_NAMEALIAS(VectorGreater);
DX_IMPL_NAMEALIAS(VectorGreaterOrEqual);
DX_IMPL_NAMEALIAS(VectorGreaterOrEqualR);
DX_IMPL_NAMEALIAS(VectorGreaterR);
DX_IMPL_NAMEALIAS(VectorLess);
DX_IMPL_NAMEALIAS(VectorLessOrEqual);
DX_IMPL_NAMEALIAS(VectorNearEqual);

//
/// Component wise
const _tVectorInsert& VectorInsert = (_tVectorInsert)DirectX::XMVectorInsert;
DX_IMPL_NAMEALIAS(VectorMergeXY);
DX_IMPL_NAMEALIAS(VectorMergeZW);
const _tVectorPermute& VectorPermute = (_tVectorPermute)DirectX::XMVectorPermute;
const _tVectorRotateLeft& VectorRotateLeft = (_tVectorRotateLeft)DirectX::XMVectorRotateLeft;;
const _tVectorRotateRight& VectorRotateRight = (_tVectorRotateRight)DirectX::XMVectorRotateRight;
DX_IMPL_NAMEALIAS(VectorSelect);
DX_IMPL_NAMEALIAS(VectorSelectControl);
const _tVectorShiftLeft& VectorShiftLeft = (_tVectorShiftLeft)DirectX::XMVectorShiftLeft;
DX_IMPL_NAMEALIAS(VectorSplatW);
DX_IMPL_NAMEALIAS(VectorSplatX);
DX_IMPL_NAMEALIAS(VectorSplatY);
DX_IMPL_NAMEALIAS(VectorSplatZ);
const _tVectorSwizzle& VectorSwizzle = (_tVectorSwizzle)DirectX::XMVectorSwizzle;

//
/// Geometric functions
DX_IMPL_NAMEALIAS(VectorBaryCentric);
DX_IMPL_NAMEALIAS(VectorBaryCentricV);
DX_IMPL_NAMEALIAS(VectorCatmullRom);
DX_IMPL_NAMEALIAS(VectorCatmullRomV);
DX_IMPL_NAMEALIAS(VectorHermite);
DX_IMPL_NAMEALIAS(VectorHermiteV);
DX_IMPL_NAMEALIAS(VectorInBounds);
DX_IMPL_NAMEALIAS(VectorInBoundsR);
DX_IMPL_NAMEALIAS(VectorLerp);
DX_IMPL_NAMEALIAS(VectorLerpV);

//
/// Vector Initialize
DX_IMPL_NAMEALIAS(VectorFalseInt);
DX_IMPL_NAMEALIAS(VectorReplicate);
DX_IMPL_NAMEALIAS(VectorReplicateInt);
DX_IMPL_NAMEALIAS(VectorReplicateIntPtr);
DX_IMPL_NAMEALIAS(VectorReplicatePtr);
DX_IMPL_NAMEALIAS(VectorSet);
DX_IMPL_NAMEALIAS(VectorSetBinaryConstant);
DX_IMPL_NAMEALIAS(VectorSetInt);
DX_IMPL_NAMEALIAS(VectorSplatConstant);
DX_IMPL_NAMEALIAS(VectorSplatConstantInt);
DX_IMPL_NAMEALIAS(VectorSplatEpsilon);
DX_IMPL_NAMEALIAS(VectorSplatInfinity);
DX_IMPL_NAMEALIAS(VectorSplatOne);
DX_IMPL_NAMEALIAS(VectorSplatQNaN);
DX_IMPL_NAMEALIAS(VectorSplatSignMask);
DX_IMPL_NAMEALIAS(VectorTrueInt);
DX_IMPL_NAMEALIAS(VectorZero);

//
/// transcendental vector function
DX_IMPL_NAMEALIAS(VectorACos);
DX_IMPL_NAMEALIAS(VectorACosEst);
DX_IMPL_NAMEALIAS(VectorASin);
DX_IMPL_NAMEALIAS(VectorASinEst);
DX_IMPL_NAMEALIAS(VectorATan);
DX_IMPL_NAMEALIAS(VectorATan2);
DX_IMPL_NAMEALIAS(VectorATan2Est);
DX_IMPL_NAMEALIAS(VectorATanEst);
DX_IMPL_NAMEALIAS(VectorCos);
DX_IMPL_NAMEALIAS(VectorCosEst);
DX_IMPL_NAMEALIAS(VectorCosH);
DX_IMPL_NAMEALIAS(VectorExp);
DX_IMPL_NAMEALIAS(VectorExp2);
DX_IMPL_NAMEALIAS(VectorExpE);
DX_IMPL_NAMEALIAS(VectorLog);
DX_IMPL_NAMEALIAS(VectorLog2);
DX_IMPL_NAMEALIAS(VectorLogE);
DX_IMPL_NAMEALIAS(VectorSin);
DX_IMPL_NAMEALIAS(VectorSinCos);
DX_IMPL_NAMEALIAS(VectorSinCosEst);
DX_IMPL_NAMEALIAS(VectorSinEst);
DX_IMPL_NAMEALIAS(VectorSinH);
DX_IMPL_NAMEALIAS(VectorTan);
DX_IMPL_NAMEALIAS(VectorTanEst);
DX_IMPL_NAMEALIAS(VectorTanH);

//
///Specialized Versions for 2D Vector

//compare
DX_IMPL_NAMEALIAS(Vector2Equal);
DX_IMPL_NAMEALIAS(Vector2EqualInt);
DX_IMPL_NAMEALIAS(Vector2EqualIntR);
DX_IMPL_NAMEALIAS(Vector2EqualR);
DX_IMPL_NAMEALIAS(Vector2Greater);
DX_IMPL_NAMEALIAS(Vector2GreaterOrEqual);
DX_IMPL_NAMEALIAS(Vector2GreaterOrEqualR);
DX_IMPL_NAMEALIAS(Vector2GreaterR);
DX_IMPL_NAMEALIAS(Vector2IsInfinite);
DX_IMPL_NAMEALIAS(Vector2IsNaN);
DX_IMPL_NAMEALIAS(Vector2Less);
DX_IMPL_NAMEALIAS(Vector2LessOrEqual);
DX_IMPL_NAMEALIAS(Vector2NearEqual);
DX_IMPL_NAMEALIAS(Vector2NotEqual);
DX_IMPL_NAMEALIAS(Vector2NotEqualInt);

//geometric
DX_IMPL_NAMEALIAS(Vector2AngleBetweenNormals);
DX_IMPL_NAMEALIAS(Vector2AngleBetweenNormalsEst);
DX_IMPL_NAMEALIAS(Vector2AngleBetweenVectors);
DX_IMPL_NAMEALIAS(Vector2ClampLength);
DX_IMPL_NAMEALIAS(Vector2ClampLengthV);
DX_IMPL_NAMEALIAS(Vector2Cross);
DX_IMPL_NAMEALIAS(Vector2Dot);
DX_IMPL_NAMEALIAS(Vector2InBounds);
DX_IMPL_NAMEALIAS(Vector2IntersectLine);
DX_IMPL_NAMEALIAS(Vector2Length);
DX_IMPL_NAMEALIAS(Vector2LengthEst);
DX_IMPL_NAMEALIAS(Vector2LengthSq);
DX_IMPL_NAMEALIAS(Vector2LinePointDistance);
DX_IMPL_NAMEALIAS(Vector2Normalize);
DX_IMPL_NAMEALIAS(Vector2NormalizeEst);
DX_IMPL_NAMEALIAS(Vector2Orthogonal);
DX_IMPL_NAMEALIAS(Vector2ReciprocalLength);
DX_IMPL_NAMEALIAS(Vector2ReciprocalLengthEst);
DX_IMPL_NAMEALIAS(Vector2Reflect);
DX_IMPL_NAMEALIAS(Vector2Refract);
DX_IMPL_NAMEALIAS(Vector2RefractV);

//vector-matrix transformation
DX_IMPL_NAMEALIAS(Vector2Transform);
DX_IMPL_NAMEALIAS(Vector2TransformCoord);
DX_IMPL_NAMEALIAS(Vector2TransformCoordStream);
DX_IMPL_NAMEALIAS(Vector2TransformNormal);
DX_IMPL_NAMEALIAS(Vector2TransformNormalStream);
DX_IMPL_NAMEALIAS(Vector2TransformStream);

//
///Specialized Versions for 3D Vector

//compare
DX_IMPL_NAMEALIAS(Vector3Equal);
DX_IMPL_NAMEALIAS(Vector3EqualInt);
DX_IMPL_NAMEALIAS(Vector3EqualIntR);
DX_IMPL_NAMEALIAS(Vector3EqualR);
DX_IMPL_NAMEALIAS(Vector3Greater);
DX_IMPL_NAMEALIAS(Vector3GreaterOrEqual);
DX_IMPL_NAMEALIAS(Vector3GreaterOrEqualR);
DX_IMPL_NAMEALIAS(Vector3GreaterR);
DX_IMPL_NAMEALIAS(Vector3IsInfinite);
DX_IMPL_NAMEALIAS(Vector3IsNaN);
DX_IMPL_NAMEALIAS(Vector3Less);
DX_IMPL_NAMEALIAS(Vector3LessOrEqual);
DX_IMPL_NAMEALIAS(Vector3NearEqual);
DX_IMPL_NAMEALIAS(Vector3NotEqual);
DX_IMPL_NAMEALIAS(Vector3NotEqualInt);

//geometric
DX_IMPL_NAMEALIAS(Vector3AngleBetweenNormals);
DX_IMPL_NAMEALIAS(Vector3AngleBetweenNormalsEst);
DX_IMPL_NAMEALIAS(Vector3AngleBetweenVectors);
DX_IMPL_NAMEALIAS(Vector3ClampLength);
DX_IMPL_NAMEALIAS(Vector3ClampLengthV);
DX_IMPL_NAMEALIAS(Vector3ComponentsFromNormal);
DX_IMPL_NAMEALIAS(Vector3Cross);
DX_IMPL_NAMEALIAS(Vector3Dot);
DX_IMPL_NAMEALIAS(Vector3InBounds);
DX_IMPL_NAMEALIAS(Vector3Length);
DX_IMPL_NAMEALIAS(Vector3LengthEst);
DX_IMPL_NAMEALIAS(Vector3LengthSq);
DX_IMPL_NAMEALIAS(Vector3LinePointDistance);
DX_IMPL_NAMEALIAS(Vector3Normalize);
DX_IMPL_NAMEALIAS(Vector3NormalizeEst);
DX_IMPL_NAMEALIAS(Vector3Orthogonal);
DX_IMPL_NAMEALIAS(Vector3ReciprocalLength);
DX_IMPL_NAMEALIAS(Vector3ReciprocalLengthEst);
DX_IMPL_NAMEALIAS(Vector3Reflect);
DX_IMPL_NAMEALIAS(Vector3Refract);
DX_IMPL_NAMEALIAS(Vector3RefractV);

//vector-matrix transformation
DX_IMPL_NAMEALIAS(Vector3InverseRotate);
DX_IMPL_NAMEALIAS(Vector3Project);
DX_IMPL_NAMEALIAS(Vector3ProjectStream);
DX_IMPL_NAMEALIAS(Vector3Rotate);
DX_IMPL_NAMEALIAS(Vector3Transform);
DX_IMPL_NAMEALIAS(Vector3TransformCoord);
DX_IMPL_NAMEALIAS(Vector3TransformCoordStream);
DX_IMPL_NAMEALIAS(Vector3TransformNormal);
DX_IMPL_NAMEALIAS(Vector3TransformNormalStream);
DX_IMPL_NAMEALIAS(Vector3TransformStream);
DX_IMPL_NAMEALIAS(Vector3Unproject);
DX_IMPL_NAMEALIAS(Vector3UnprojectStream);

//
///Specialized Versions for 4D Vector

//compare
DX_IMPL_NAMEALIAS(Vector4Equal);
DX_IMPL_NAMEALIAS(Vector4EqualInt);
DX_IMPL_NAMEALIAS(Vector4EqualIntR);
DX_IMPL_NAMEALIAS(Vector4EqualR);
DX_IMPL_NAMEALIAS(Vector4Greater);
DX_IMPL_NAMEALIAS(Vector4GreaterOrEqual);
DX_IMPL_NAMEALIAS(Vector4GreaterOrEqualR);
DX_IMPL_NAMEALIAS(Vector4GreaterR);
DX_IMPL_NAMEALIAS(Vector4IsInfinite);
DX_IMPL_NAMEALIAS(Vector4IsNaN);
DX_IMPL_NAMEALIAS(Vector4Less);
DX_IMPL_NAMEALIAS(Vector4LessOrEqual);
DX_IMPL_NAMEALIAS(Vector4NearEqual);
DX_IMPL_NAMEALIAS(Vector4NotEqual);
DX_IMPL_NAMEALIAS(Vector4NotEqualInt);

//geometric
DX_IMPL_NAMEALIAS(Vector4AngleBetweenNormals);
DX_IMPL_NAMEALIAS(Vector4AngleBetweenNormalsEst);
DX_IMPL_NAMEALIAS(Vector4AngleBetweenVectors);
DX_IMPL_NAMEALIAS(Vector4ClampLength);
DX_IMPL_NAMEALIAS(Vector4ClampLengthV);
DX_IMPL_NAMEALIAS(Vector4Cross);
DX_IMPL_NAMEALIAS(Vector4Dot);
DX_IMPL_NAMEALIAS(Vector4InBounds);
DX_IMPL_NAMEALIAS(Vector4Length);
DX_IMPL_NAMEALIAS(Vector4LengthEst);
DX_IMPL_NAMEALIAS(Vector4LengthSq);
DX_IMPL_NAMEALIAS(Vector4Normalize);
DX_IMPL_NAMEALIAS(Vector4NormalizeEst);
DX_IMPL_NAMEALIAS(Vector4Orthogonal);
DX_IMPL_NAMEALIAS(Vector4ReciprocalLength);
DX_IMPL_NAMEALIAS(Vector4ReciprocalLengthEst);
DX_IMPL_NAMEALIAS(Vector4Reflect);
DX_IMPL_NAMEALIAS(Vector4Refract);
DX_IMPL_NAMEALIAS(Vector4RefractV);

//Vector-matrix transform#
DX_IMPL_NAMEALIAS(Vector4Transform);
DX_IMPL_NAMEALIAS(Vector4TransformStream);

ENDNAMESPACE

#endif
