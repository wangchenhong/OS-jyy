#
# SPDX-License-Identifier: BSD-2-Clause
#
# Copyright (c) 2024 Ventana Micro Systems Inc.
#
# Authors:
#   Anup Patel <apatel@ventanamicro.com>
#

carray-fdt_early_drivers-$(CONFIG_FDT_SUSPEND_RPMI) += fdt_suspend_rpmi
libsbiutils-objs-$(CONFIG_FDT_SUSPEND_RPMI) += suspend/fdt_suspend_rpmi.o
