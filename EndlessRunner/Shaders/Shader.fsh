//
//  Shader.fsh
//  EndlessRunner
//
//  Created by Marcos Marti Nacher on 2/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

precision mediump float;

uniform vec3 u_color;
uniform sampler2D u_texture;
uniform vec2 u_uv;

void main()
{
    //gl_FragColor = vec4(texture2D(u_texture, u_uv).rgb, 1.0);
    gl_FragColor = vec4(u_color, 1.0);
}
