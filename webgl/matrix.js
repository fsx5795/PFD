/**
 @brief 叉乘（行向量右乘）
 @param pointMat 需要计算的矩阵
 @param calculateMat 参于计算的矩阵
 @return 计算的结果
 */
function crossProduct(pointMat, calculateMat) {
    //右乘需先对旋转矩阵进行转置，否则当前旋转矩阵的形式为列向量左乘的旋转矩阵
    const transposedMat = calculateMat[0].map((_, col) => calculateMat.map(row => row[col]) )
    const pointMatRows = pointMat.length
    const targetMat = Array.from({ length: pointMatRows }, () => Array(2))
    for (let i = 0; i < pointMatRows; ++i) {
        targetMat[i][0] = pointMat[i].reduce((sum, value, index) => sum + value * transposedMat[index][0], 0)
        targetMat[i][1] = pointMat[i].reduce((sum, value, index) => sum + value * transposedMat[index][1], 0)
    }
    return targetMat
}

export function rotate(mat, angle) {
    //旋转矩阵
    const coefficientMat = Array.from({ length: 2 }, () => Array(2))
    const rad = Math.PI / 180 * angle
    coefficientMat[0][0] = Math.cos(rad)
    coefficientMat[0][1] = -Math.sin(rad)
    coefficientMat[1][0] = Math.sin(rad)
    coefficientMat[1][1] = Math.cos(rad)
    return crossProduct(mat, coefficientMat)
}