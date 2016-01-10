//////////////////////////////////////////////////
////////// Definition of constant buffer
////////////////////////////////////////


ConstantBuffer global ViewProjectionMatrices {
	uniform mat4 m4_ViewProjection;
	uniform mat4 m4_Projection;
	uniform mat4 m4_ProjectionI;
	uniform mat4 m4_View;
	uniform mat4 m4_ViewIT;
	uniform vec3 v3_EyePos : EYEPOS;
	uniform float dummy;
	uniform vec3 v3_ViewDirection;
	uniform float dummy_2;
	uniform vec2 v2_ClippingDistance;
	uniform ivec2 iv2_ViewportSize;
}
ConstantBuffer global ModelMatrices {
	uniform mat4 m4_Model;
	uniform mat4 m4_ModelView;
	uniform mat4 m4_ModelViewProjection;
	uniform mat4 m4_ModelIT;
	uniform mat4 m4_ModelViewIT;
}		