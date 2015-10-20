//
//  Shader.vsh
//  EndlessRunner
//
//  Created by Marcos Marti Nacher on 2/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//


attribute vec4 a_position;

uniform mat4 u_modelViewProjectionMatrix;

void main()
{
	gl_Position = u_modelViewProjectionMatrix * a_position;
}
