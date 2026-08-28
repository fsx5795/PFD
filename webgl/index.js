import { rotate } from './matrix.js'

const canvas = document.querySelector('canvas')
canvas.width = canvas.clientWidth
canvas.height = canvas.clientHeight

const gl = canvas.getContext('webgl2')

async function createShader(gl, type, source) {
    const shader = gl.createShader(type)
    const res = await fetch(source)
    const src = await res.text()

    gl.shaderSource(shader, src)
    gl.compileShader(shader)
    if (gl.getShaderParameter(shader, gl.COMPILE_STATUS))
        return shader

    console.log(gl.getShaderInfoLog(shader))
    gl.deleteShader(shader)
    return null
}

if (gl instanceof WebGL2RenderingContext) {
    const vertextShader = await createShader(gl, gl.VERTEX_SHADER, 'vert.vert')
    const fragmentShader = await createShader(gl, gl.FRAGMENT_SHADER, 'frag.frag')

    const program = gl.createProgram()
    gl.attachShader(program, vertextShader)
    gl.attachShader(program, fragmentShader)
    gl.linkProgram(program)

    if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
        console.log(gl.getProgramInfoLog(program))
        gl.deleteProgram(program)
    } else {
        gl.useProgram(program)

        const rightTopPoints = Array.from({ length: 90 }, () => Array(2).fill(0))
        for (let i = 0; i < 90; ++i) {
            rightTopPoints[i][0] = Math.cos(Math.PI / 180 * i)
            rightTopPoints[i][1] = Math.sin(Math.PI / 180 * i)
        }

        const leftTopPoints = rotate(rightTopPoints, 90)

        //启用 glsl 中的属性，否则这个属性值会是一个常量
        const pos = gl.getAttribLocation(program, 'pos')
        gl.enableVertexAttribArray(pos)

        gl.viewport(0, 0, gl.canvas.width, gl.canvas.height)
        gl.clearColor(0.0, 0.0, 0.0, 1.0)
        gl.clear(gl.COLOR_BUFFER_BIT)

        const rightTopVbo = gl.createBuffer()
        gl.bindBuffer(gl.ARRAY_BUFFER, rightTopVbo)
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(rightTopPoints.flat()), gl.STATIC_DRAW)

        //设置属性如何从缓冲区取出数据
        gl.vertexAttribPointer(pos, 2, gl.FLOAT, false, 0, 0)
        gl.drawArrays(gl.LINE_STRIP, 0, 90)

        const leftTopVbo = gl.createBuffer()
        gl.bindBuffer(gl.ARRAY_BUFFER, leftTopVbo)
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(leftTopPoints.flat()), gl.STATIC_DRAW)

        //设置属性如何从缓冲区取出数据
        gl.vertexAttribPointer(pos, 2, gl.FLOAT, false, 0, 0)
        gl.drawArrays(gl.LINE_STRIP, 0, 90)

        /*
        const vao = gl.createVertexArray()
        gl.bindVertexArray(vao)
        */
    }
}