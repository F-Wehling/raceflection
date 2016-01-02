//////////////////////////////////////////////////
////////// Definition of constant buffer
////////////////////////////////////////


ConstantBuffer global ViewProjectionMatrices {
	uniform mat4 m4_ViewProjection;
	uniform mat4 m4_Projection;
	uniform mat4 m4_View;
	uniform mat4 m4_ViewIT;
	uniform vec3 v3_eyePos : EYEPOS;
	uniform float dummy;
}
ConstantBuffer global ModelMatrices {
	uniform mat4 m4_Model;
	uniform mat4 m4_ModelView;
	uniform mat4 m4_ModelViewProjection;
	uniform mat4 m4_ModelIT;
}		