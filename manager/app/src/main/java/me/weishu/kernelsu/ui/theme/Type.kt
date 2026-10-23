package me.weishu.kernelsu.ui.theme

import androidx.compose.material3.Typography
import androidx.compose.ui.text.TextStyle
import androidx.compose.ui.text.font.Font
import androidx.compose.ui.text.font.FontFamily
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.sp
import me.weishu.kernelsu.R
import top.yukonga.miuix.kmp.theme.TextStyles
import top.yukonga.miuix.kmp.theme.defaultTextStyles

/**
 * 内置小赖字体（Xiaolai）。
 * 作为应用全局默认字体，不做运行时切换。
 */
val XiaolaiFontFamily = FontFamily(
    Font(R.font.xiaolai, weight = FontWeight.Normal)
)

private fun TextStyle.withXiaolai() = copy(fontFamily = XiaolaiFontFamily)

/**
 * Material 3 Typography：所有文字样式使用小赖字体。
 * 由 [MaterialKernelSUTheme] 通过 typography = Typography 使用。
 */
val Typography = Typography().let { base ->
    Typography(
        displayLarge = base.displayLarge.withXiaolai(),
        displayMedium = base.displayMedium.withXiaolai(),
        displaySmall = base.displaySmall.withXiaolai(),
        headlineLarge = base.headlineLarge.withXiaolai(),
        headlineMedium = base.headlineMedium.withXiaolai(),
        headlineSmall = base.headlineSmall.withXiaolai(),
        titleLarge = base.titleLarge.withXiaolai(),
        titleMedium = base.titleMedium.withXiaolai(),
        titleSmall = base.titleSmall.withXiaolai(),
        bodyLarge = base.bodyLarge.withXiaolai().copy(
            fontSize = 16.sp,
            lineHeight = 24.sp,
            letterSpacing = 0.5.sp,
            fontWeight = FontWeight.Normal,
        ),
        bodyMedium = base.bodyMedium.withXiaolai(),
        bodySmall = base.bodySmall.withXiaolai(),
        labelLarge = base.labelLarge.withXiaolai(),
        labelMedium = base.labelMedium.withXiaolai(),
        labelSmall = base.labelSmall.withXiaolai(),
    )
}

/**
 * MIUIX TextStyles：在默认字号/字重基础上只替换 fontFamily。
 * 由 [MiuixKernelSUTheme] 通过 textStyles = XiaolaiMiuixTextStyles 使用。
 */
val XiaolaiMiuixTextStyles: TextStyles = defaultTextStyles().let { base ->
    defaultTextStyles(
        main = base.main.withXiaolai(),
        paragraph = base.paragraph.withXiaolai(),
        body1 = base.body1.withXiaolai(),
        body2 = base.body2.withXiaolai(),
        button = base.button.withXiaolai(),
        footnote1 = base.footnote1.withXiaolai(),
        footnote2 = base.footnote2.withXiaolai(),
        headline1 = base.headline1.withXiaolai(),
        headline2 = base.headline2.withXiaolai(),
        subtitle = base.subtitle.withXiaolai(),
        title1 = base.title1.withXiaolai(),
        title2 = base.title2.withXiaolai(),
        title3 = base.title3.withXiaolai(),
        title4 = base.title4.withXiaolai(),
    )
}