/**
 @brief 行向量右乘
 @param pointMat 需要计算的矩阵
 @param calculateMat 参于计算的方阵
 @return 计算的结果
 */
function matrixMultply(pointMat, calculateMat) {
    const pointMatRows = pointMat.length
    //参于计算的方阵的阶数
    const n = calculateMat.length
    //计算的结果
    const targetMat = Array.from({ length: pointMatRows }, () => Array(n))
    for (let i = 0; i < pointMatRows; ++i) {
        for (let j = 0; j < n; ++j)
            //计算内积
            targetMat[i][j] = pointMat[i].reduce((sum, value, index) => sum + value * calculateMat[index][j], 0)
    }
    return targetMat
}

//旋转变换
export function rotate(mat, angle) {
    //旋转矩阵
    const coefficientMat = Array.from({ length: 2 }, () => Array(2))
    const rad = Math.PI / 180 * angle
    coefficientMat[0][0] = Math.cos(rad)
    coefficientMat[0][1] = -Math.sin(rad)
    coefficientMat[1][0] = Math.sin(rad)
    coefficientMat[1][1] = Math.cos(rad)

    //右乘需先对旋转矩阵进行转置，否则当前旋转矩阵的形式为列向量左乘的旋转矩阵
    const transposedMat = coefficientMat[0].map((_, col) => coefficientMat.map(row => row[col]) )
    return matrixMultply(mat, transposedMat)
}

//仿射变换
export function affine(mat, point, axis) {
    const allowed = new Set(['x', 'y'])
    if (!allowed.has(axis))
        throw new TypeError(`参数必须为 'x' 或 'y'，但收到了 "${axis}"`)

    //先创建单位矩阵
    const affineMat = Array.from({ length: 3 }, (_, i) => Array(3).fill(0).with(i, 1))
    //齐次仿射矩阵
    affineMat[0][0] = axis === 'x' ? -1 : 1
    affineMat[0][2] = axis === 'x' ? 2 * point : 0
    affineMat[1][1] = axis === 'y' ? -1 : 1
    affineMat[1][2] = axis === 'y' ? 2 * point : 0

    //补齐参于仿射变换计算的第三列
    const sourceMat = mat.map(row => [...row, 1])
    //右乘需先对仿射矩阵进行转置，否则当前仿射矩阵的形式为列向量左乘的仿射矩阵
    const transposedMat = affineMat[0].map((_, col) => affineMat.map(row => row[col]))

    //仿射变换
    const targetMat = matrixMultply(sourceMat, transposedMat)
    targetMat.forEach(row => row.pop())
    return targetMat
}

//缩放变换
export function scale(mat, ratio) {
    //先创建单位矩阵
    const scaleMat = Array.from({ length: 2 }, (_, i) => Array(2).fill(0).with(i, 1))
    //改成缩放矩阵
    scaleMat[0][0] = ratio
    scaleMat[1][1] = ratio
    //右乘需先对仿射矩阵进行转置，否则当前仿射矩阵的形式为列向量左乘的仿射矩阵
    const transposedMat = scaleMat[0].map((_, col) => scaleMat.map(row => row[col]))
    return matrixMultply(mat, transposedMat)
}