//
//  Shader.fsh
//  EndlessRunner
//
//  Created by Marcos Marti Nacher on 2/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

precision mediump float;

uniform vec3 u_color;

void main()
{
    gl_FragColor = vec4(u_color, 1.0);
}
