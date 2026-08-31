import { rotate, affine, scale } from './matrix.js'

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

    if (vertextShader === null || fragmentShader === null)
        throw new Error('渲染器创建失败')

    const program = gl.createProgram()
    gl.attachShader(program, vertextShader)
    gl.attachShader(program, fragmentShader)
    gl.linkProgram(program)

    if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
        console.log(gl.getProgramInfoLog(program))
        gl.deleteProgram(program)
    } else {
        gl.useProgram(program)

        const deg = Math.PI / 180
        //右上角圆弧
        const rightTopPoints = Array.from({ length: 90 }, (_, i) => {
            const rad = deg * i
            return [Math.cos(rad), Math.sin(rad)]
        })

        //左上角圆弧
        const leftTopPoints = rotate(rightTopPoints, 90)

        let topPoints = [...leftTopPoints, ...rightTopPoints]
        topPoints = scale(topPoints, 0.1)
        //右下角圆弧
        const buttomPoints = affine(topPoints, 0, 'y')

        gl.viewport(0, 0, gl.canvas.width, gl.canvas.height)
        gl.clearColor(0.0, 0.0, 0.0, 1.0)
        gl.clear(gl.COLOR_BUFFER_BIT)
        
        const vao = gl.createVertexArray()
        gl.bindVertexArray(vao)
        //启用 glsl 中的属性，否则这个属性值会是一个常量
        const pos = gl.getAttribLocation(program, 'pos')
        gl.enableVertexAttribArray(pos)

        const arcs = [topPoints, buttomPoints]
        const vertices = new Float32Array(arcs.flat(Infinity))
        const vbo = gl.createBuffer()
        gl.bindBuffer(gl.ARRAY_BUFFER, vbo)
        gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW)
        gl.vertexAttribPointer(pos, 2, gl.FLOAT, false, 0, 0)

        //分段绘制，防止出现额外的连线
        gl.drawArrays(gl.LINE_STRIP, 0, 90)
        gl.drawArrays(gl.LINE_STRIP, 90, 90)
        gl.drawArrays(gl.LINE_STRIP, 90 * 2, 90)
        gl.drawArrays(gl.LINE_STRIP, 90 * 3, 90)
    }
}