
    .macro End
    .long 0
    .endm

    .macro WaitFrame
    .long 1
    .endm

    .macro SetOriginalPosition
    .long 2
    .endm

    .macro SetVarIf readType1, varIndex1, valueOrVarIndex2, comparisonType, readType2, destVarIndex, newValue
    .long 3
    .long \readType1
    .long \varIndex1
    .long \valueOrVarIndex2
    .long \comparisonType
    .long \readType2
    .long \destVarIndex
    .long \newValue
    .endm

    .macro SetVar destVarIndex, value
    .long 4
    .long \destVarIndex
    .long \value
    .endm

    .macro CopyVar destVarIndex, originIndex
    .long 5
    .long \destVarIndex
    .long \originIndex
    .endm

    .macro Add destVarIndex, readType, varIndex1, valueOrVarIndex2
    .long 6
    .long \destVarIndex
    .long \readType
    .long \varIndex1
    .long \valueOrVarIndex2
    .endm

    .macro Multiply destVarIndex, readType, varIndex1, valueOrVarIndex2
    .long 7
    .long \destVarIndex
    .long \readType
    .long \varIndex1
    .long \valueOrVarIndex2
    .endm

    .macro Subtract destVarIndex, readType1, readType2, valueOrVarIndex1, valueOrVarIndex2
    .long 8
    .long \destVarIndex
    .long \readType1
    .long \readType2
    .long \valueOrVarIndex1
    .long \valueOrVarIndex2
    .endm

    .macro Divide destVarIndex, readType1, readType2, valueOrVarIndex1, valueOrVarIndex2
    .long 9
    .long \destVarIndex
    .long \readType1
    .long \readType2
    .long \valueOrVarIndex1
    .long \valueOrVarIndex2
    .endm

    .macro Modulo destVarIndex, readType1, readType2, valueOrVarIndex1, valueOrVarIndex2
    .long 10
    .long \destVarIndex
    .long \readType1
    .long \readType2
    .long \valueOrVarIndex1
    .long \valueOrVarIndex2
    .endm

    .macro Loop loopMax
    .long 11
    .long \loopMax
    .endm

    .macro LoopEnd
    .long 12
    .endm

    .macro SetSpriteAttribute, spriteAttribute, originIndex
    .long 13
    .long \spriteAttribute
    .long \originIndex
    .endm

    .macro AddSpriteAttribute spriteAttribute, originIndex
    .long 14
    .long \spriteAttribute
    .long \originIndex
    .endm

    .macro UpdateSpriteAttribute spriteAttribute, readType, valueOrVarIndex, attributeUpdateType
    .long 15
    .long \spriteAttribute
    .long \readType
    .long \valueOrVarIndex
    .long \attributeUpdateType
    .endm

    .macro Sin destVarIndex, radiansIndex, readTypeAmplitude, amplitude, readTypeOffset, offset
    .long 16
    .long \destVarIndex
    .long \radiansIndex
    .long \readTypeAmplitude
    .long \amplitude
    .long \readTypeOffset
    .long \offset
    .endm

    .macro Cos destVarIndex, radiansIndex, readTypeAmplitude, amplitude, readTypeOffset, offset
    .long 17
    .long \destVarIndex
    .long \radiansIndex
    .long \readTypeAmplitude
    .long \amplitude
    .long \readTypeOffset
    .long \offset
    .endm

    .macro SetTranslation varIndex, translationType
    .long 18
    .long \varIndex
    .long \translationType
    .endm

    .macro AddTranslation varIndex, translationType
    .long 19
    .long \varIndex
    .long \translationType
    .endm

    .macro UpdateAttribute animAttribute, readType, valueOrVarIndex, updateType
    .long 20
    .long \animAttribute
    .long \readType
    .long \valueOrVarIndex
    .long \updateType
    .endm

    .macro ApplyTranslation
    .long 21
    .endm

    .macro ApplyScaleAndRotation
    .long 22
    .endm

    .macro SetOffset varIndex, offsetType
    .long 23
    .long \varIndex
    .long \offsetType
    .endm

    .macro WaitTransform
    .long 24
    .endm

    .macro SetYNormalization yNormalizationType
    .long 25
    .long \yNormalizationType
    .endm

    .macro TransformCurve calcType, startDelay, curveType, transformType, amplitude, radians, offset, steps
    .long 26
    .long \calcType
    .long \startDelay
    .long \curveType
    .long \transformType
    .long \amplitude
    .long \radians
    .long \offset
    .long \steps
    .endm

    .macro TransformCurveEven calcType, startDelay, curveType, transformType, amplitude, radians, offset, steps
    .long 27
    .long \calcType
    .long \startDelay
    .long \curveType
    .long \transformType
    .long \amplitude
    .long \radians
    .long \offset
    .long \steps
    .endm

    .macro TransformLinear  calcType, startDelay, transformType, speed, acceleration, steps
    .long 28
    .long \calcType
    .long \startDelay
    .long \transformType
    .long \speed
    .long \acceleration
    .long \steps
    .endm

    .macro TransformLinearEven calcType, startDelay, transformType, distance, steps
    .long 29
    .long \calcType
    .long \startDelay
    .long \transformType
    .long \distance
    .long \steps
    .endm

    .macro TransformLinearBounded calcType, startDelay, transformType, speed, acceleration, maxDistance
    .long 30
    .long \calcType
    .long \startDelay
    .long \transformType
    .long \speed
    .long \acceleration
    .long \maxDistance
    .endm

    .macro SetStartDelay delay
    .long 31
    .long \delay
    .endm

    .macro Fade initAlpha, targetAlpha, delay, color
    .long 32
    .long \initAlpha
    .long \targetAlpha
    .long \delay
    .long \color
    .endm

    .macro WaitFade
    .long 33
    .endm
