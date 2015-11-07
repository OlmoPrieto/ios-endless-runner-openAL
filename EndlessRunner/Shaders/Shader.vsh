//
//  Shader.vsh
//  EndlessRunner
//
//  Created by Marcos Marti Nacher on 2/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

attribute vec4 a_position;
attribute vec2 a_uv;

uniform mat4 u_modelViewProjectionMatrix;

varying vec2 o_uv;

void main()
{
	gl_Position = u_modelViewProjectionMatrix * a_position;

    o_uv = a_uv;
}
