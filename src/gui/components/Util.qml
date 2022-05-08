// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2022 Wolfgang Frisch
import QtQuick 2.0

Item {
    function rangemap(x, src_a, src_b, dst_a, dst_b) {
        return dst_a + ((dst_b - dst_a) / (src_b - src_a)) * (x - src_a)
    }
}
