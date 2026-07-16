	.text
	.amdgcn_target "amdgcn-amd-amdhsa--gfx1010:xnack-"
	.amdhsa_code_object_version 5
	.protected	mul                     ; -- Begin function mul
	.globl	mul
	.p2align	8
	.type	mul,@function
mul:                                    ; @mul
; %bb.0:
	s_clause 0x1
	s_load_dword s17, s[4:5], 0x20
	s_load_dwordx8 s[8:15], s[4:5], 0x0
	s_lshl_b32 s16, s7, 4
	v_mov_b32_e32 v49, 0
	v_mov_b32_e32 v48, 0
	v_mov_b32_e32 v47, 0
	v_mov_b32_e32 v46, 0
	v_mov_b32_e32 v45, 0
	v_mov_b32_e32 v44, 0
	v_mov_b32_e32 v43, 0
	v_mov_b32_e32 v42, 0
	v_mov_b32_e32 v41, 0
	v_mov_b32_e32 v40, 0
	v_mov_b32_e32 v39, 0
	v_mov_b32_e32 v38, 0
	v_mov_b32_e32 v37, 0
	v_mov_b32_e32 v36, 0
	v_mov_b32_e32 v35, 0
	v_mov_b32_e32 v34, 0
	v_mov_b32_e32 v33, 0
	s_waitcnt lgkmcnt(0)
	s_mul_hi_u32 s0, s17, 0x3e0f83e1
	s_sub_i32 s1, 0, s17
	s_lshr_b32 s0, s0, 4
	s_lshr_b32 s15, s15, 1
	s_mul_i32 s2, s0, 0xffffffbe
	v_mul_u32_u24_e32 v54, 3, v0
	s_cmp_lg_u32 s2, s1
	v_mov_b32_e32 v32, 0
	s_cselect_b32 s1, -1, 0
	v_mov_b32_e32 v31, 0
	s_cmp_lg_u32 s1, 0
	v_mov_b32_e32 v30, 0
	v_mov_b32_e32 v29, 0
	v_mov_b32_e32 v28, 0
	v_mov_b32_e32 v27, 0
	v_mov_b32_e32 v26, 0
	v_mov_b32_e32 v25, 0
	v_mov_b32_e32 v24, 0
	v_mov_b32_e32 v23, 0
	v_mov_b32_e32 v22, 0
	v_mov_b32_e32 v21, 0
	v_mov_b32_e32 v20, 0
	v_mov_b32_e32 v19, 0
	v_mov_b32_e32 v18, 0
	v_mov_b32_e32 v17, 0
	v_mov_b32_e32 v16, 0
	v_mov_b32_e32 v15, 0
	v_mov_b32_e32 v14, 0
	v_mov_b32_e32 v13, 0
	v_mov_b32_e32 v12, 0
	v_mov_b32_e32 v11, 0
	v_mov_b32_e32 v10, 0
	v_mov_b32_e32 v9, 0
	v_mov_b32_e32 v8, 0
	v_mov_b32_e32 v7, 0
	v_mov_b32_e32 v6, 0
	v_mov_b32_e32 v5, 0
	v_mov_b32_e32 v4, 0
	v_mov_b32_e32 v3, 0
	v_mov_b32_e32 v2, 0
	s_addc_u32 s18, s0, 0
	s_mul_i32 s6, s6, 48
	s_cmp_eq_u32 s18, 0
	s_mov_b32 s2, 0
	s_cbranch_scc1 .LBB0_49
; %bb.1:
	s_load_dword s0, s[4:5], 0x34
	s_lshr_b32 s4, s17, 1
	s_add_i32 s1, s16, 1
	s_mov_b32 s3, s2
	s_mul_i32 s21, s4, s1
	v_mov_b32_e32 v2, s2
	v_mov_b32_e32 v3, s3
	s_movk_i32 s28, 0x840
	v_lshlrev_b32_e32 v55, 3, v54
	v_mad_u32_u24 v61, v1, s28, 0x6300
	v_mov_b32_e32 v51, 0
	v_mov_b32_e32 v5, v3
	v_mov_b32_e32 v7, v3
	v_mov_b32_e32 v9, v3
	v_mov_b32_e32 v11, v3
	v_mov_b32_e32 v13, v3
	v_mov_b32_e32 v15, v3
	v_mov_b32_e32 v17, v3
	v_mov_b32_e32 v19, v3
	v_mov_b32_e32 v21, v3
	s_waitcnt lgkmcnt(0)
	s_and_b32 s1, s0, 0xffff
	s_lshr_b32 s0, s0, 16
	v_mad_u32_u24 v0, v1, s1, v0
	v_mov_b32_e32 v23, v3
	v_mov_b32_e32 v25, v3
	v_mov_b32_e32 v27, v3
	v_mov_b32_e32 v29, v3
	v_lshlrev_b32_e32 v56, 3, v0
	v_mov_b32_e32 v31, v3
	v_mov_b32_e32 v33, v3
	v_mov_b32_e32 v35, v3
	v_mov_b32_e32 v37, v3
	v_mov_b32_e32 v39, v3
	v_mov_b32_e32 v41, v3
	v_mov_b32_e32 v43, v3
	v_mov_b32_e32 v45, v3
	v_mov_b32_e32 v47, v3
	v_mov_b32_e32 v49, v3
	s_mul_i32 s3, s0, s1
	v_cmp_gt_u32_e64 s0, 33, v0
	v_cmp_gt_u32_e64 s1, 48, v0
	v_add_nc_u32_e32 v57, 0x6300, v56
	v_add_nc_u32_e32 v58, 0x6408, v56
	v_add_nc_u32_e32 v59, 0x6510, v56
	v_add_nc_u32_e32 v60, 0x6618, v56
	v_mov_b32_e32 v4, v2
	v_mov_b32_e32 v6, v2
	v_mov_b32_e32 v8, v2
	v_mov_b32_e32 v10, v2
	v_mov_b32_e32 v12, v2
	v_mov_b32_e32 v14, v2
	v_mov_b32_e32 v16, v2
	v_mov_b32_e32 v18, v2
	v_mov_b32_e32 v20, v2
	v_mov_b32_e32 v22, v2
	v_mov_b32_e32 v24, v2
	v_mov_b32_e32 v26, v2
	v_mov_b32_e32 v28, v2
	v_mov_b32_e32 v30, v2
	v_mov_b32_e32 v32, v2
	v_mov_b32_e32 v34, v2
	v_mov_b32_e32 v36, v2
	v_mov_b32_e32 v38, v2
	v_mov_b32_e32 v40, v2
	v_mov_b32_e32 v42, v2
	v_mov_b32_e32 v44, v2
	v_mov_b32_e32 v46, v2
	v_mov_b32_e32 v48, v2
	s_add_i32 s22, s16, 2
	s_add_i32 s23, s16, 3
	s_lshl_b32 s24, s15, 1
	s_mul_i32 s27, s15, 3
	s_mul_i32 s25, s7, s4
	s_mul_i32 s5, s15, 0x42
	s_lshl_b32 s19, s15, 2
	s_add_i32 s20, s15, s6
	s_lshl_b32 s7, s4, 2
	s_mul_i32 s22, s4, s22
	s_mul_i32 s23, s4, s23
	s_add_i32 s24, s6, s24
	s_lshl_b32 s25, s25, 4
	s_lshl_b32 s26, s3, 3
	s_add_i32 s27, s6, s27
	s_mov_b32 s28, s6
	s_mov_b32 s29, 0
.LBB0_2:                                ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB0_4 Depth 2
                                        ;       Child Loop BB0_7 Depth 3
                                        ;       Child Loop BB0_12 Depth 3
                                        ;       Child Loop BB0_17 Depth 3
                                        ;       Child Loop BB0_22 Depth 3
                                        ;     Child Loop BB0_26 Depth 2
                                        ;       Child Loop BB0_29 Depth 3
                                        ;       Child Loop BB0_33 Depth 3
                                        ;       Child Loop BB0_39 Depth 3
                                        ;       Child Loop BB0_43 Depth 3
                                        ;     Child Loop BB0_47 Depth 2
	v_mov_b32_e32 v62, v60
	v_mov_b32_e32 v63, v59
	v_mov_b32_e32 v64, v58
	v_mov_b32_e32 v65, v57
	s_mov_b32 s30, 0
	s_mov_b32 s31, s23
	s_mov_b32 s33, s22
	s_mov_b32 s34, s21
	s_mov_b32 s35, s25
	s_branch .LBB0_4
.LBB0_3:                                ; %Flow493
                                        ;   in Loop: Header=BB0_4 Depth=2
	s_or_b32 exec_lo, exec_lo, s36
	v_add_nc_u32_e32 v65, 0x420, v65
	v_add_nc_u32_e32 v64, 0x420, v64
	v_add_nc_u32_e32 v63, 0x420, v63
	v_add_nc_u32_e32 v62, 0x420, v62
	s_add_i32 s30, s30, 4
	s_add_i32 s35, s35, s7
	s_add_i32 s34, s34, s7
	s_add_i32 s33, s33, s7
	s_add_i32 s31, s31, s7
	s_cmp_lg_u32 s30, 16
	s_cbranch_scc0 .LBB0_24
.LBB0_4:                                ;   Parent Loop BB0_2 Depth=1
                                        ; =>  This Loop Header: Depth=2
                                        ;       Child Loop BB0_7 Depth 3
                                        ;       Child Loop BB0_12 Depth 3
                                        ;       Child Loop BB0_17 Depth 3
                                        ;       Child Loop BB0_22 Depth 3
	s_and_saveexec_b32 s36, s0
	s_cbranch_execz .LBB0_9
; %bb.5:                                ;   in Loop: Header=BB0_4 Depth=2
	s_or_b32 s37, s30, s16
	v_mov_b32_e32 v66, v65
	v_mov_b32_e32 v67, v0
	s_cmp_lt_u32 s37, s14
	s_mov_b32 s37, 0
	s_cselect_b32 s38, -1, 0
	s_branch .LBB0_7
.LBB0_6:                                ;   in Loop: Header=BB0_7 Depth=3
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s39
	v_add_nc_u32_e32 v67, s3, v67
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v66, v[52:53]
	v_add_nc_u32_e32 v66, s26, v66
	v_cmp_lt_u32_e32 vcc_lo, 32, v67
	s_or_b32 s37, vcc_lo, s37
	s_andn2_b32 exec_lo, exec_lo, s37
	s_cbranch_execz .LBB0_9
.LBB0_7:                                ;   Parent Loop BB0_2 Depth=1
                                        ;     Parent Loop BB0_4 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	v_add_nc_u32_e32 v50, s2, v67
	v_mov_b32_e32 v52, 0
	v_mov_b32_e32 v53, 0
	v_cmp_gt_u32_e32 vcc_lo, s4, v50
	s_and_b32 s40, s38, vcc_lo
	s_and_saveexec_b32 s39, s40
	s_cbranch_execz .LBB0_6
; %bb.8:                                ;   in Loop: Header=BB0_7 Depth=3
	v_add_nc_u32_e32 v50, s35, v67
	v_lshlrev_b64 v[52:53], 3, v[50:51]
	v_add_co_u32 v52, vcc_lo, s8, v52
	v_add_co_ci_u32_e32 v53, vcc_lo, s9, v53, vcc_lo
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[52:53], v[52:53], off
	s_branch .LBB0_6
.LBB0_9:                                ; %Flow499
                                        ;   in Loop: Header=BB0_4 Depth=2
	s_or_b32 exec_lo, exec_lo, s36
	s_and_saveexec_b32 s36, s0
	s_cbranch_execz .LBB0_14
; %bb.10:                               ;   in Loop: Header=BB0_4 Depth=2
	s_or_b32 s37, s30, s16
	v_mov_b32_e32 v66, v64
	s_or_b32 s37, s37, 1
	v_mov_b32_e32 v67, v0
	s_cmp_lt_u32 s37, s14
	s_mov_b32 s37, 0
	s_cselect_b32 s38, -1, 0
	s_branch .LBB0_12
.LBB0_11:                               ;   in Loop: Header=BB0_12 Depth=3
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s39
	v_add_nc_u32_e32 v67, s3, v67
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v66, v[52:53]
	v_add_nc_u32_e32 v66, s26, v66
	v_cmp_lt_u32_e32 vcc_lo, 32, v67
	s_or_b32 s37, vcc_lo, s37
	s_andn2_b32 exec_lo, exec_lo, s37
	s_cbranch_execz .LBB0_14
.LBB0_12:                               ;   Parent Loop BB0_2 Depth=1
                                        ;     Parent Loop BB0_4 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	v_add_nc_u32_e32 v50, s2, v67
	v_mov_b32_e32 v52, 0
	v_mov_b32_e32 v53, 0
	v_cmp_gt_u32_e32 vcc_lo, s4, v50
	s_and_b32 s40, s38, vcc_lo
	s_and_saveexec_b32 s39, s40
	s_cbranch_execz .LBB0_11
; %bb.13:                               ;   in Loop: Header=BB0_12 Depth=3
	v_add_nc_u32_e32 v50, s34, v67
	v_lshlrev_b64 v[52:53], 3, v[50:51]
	v_add_co_u32 v52, vcc_lo, s8, v52
	v_add_co_ci_u32_e32 v53, vcc_lo, s9, v53, vcc_lo
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[52:53], v[52:53], off
	s_branch .LBB0_11
.LBB0_14:                               ; %Flow497
                                        ;   in Loop: Header=BB0_4 Depth=2
	s_or_b32 exec_lo, exec_lo, s36
	s_and_saveexec_b32 s36, s0
	s_cbranch_execz .LBB0_19
; %bb.15:                               ;   in Loop: Header=BB0_4 Depth=2
	s_or_b32 s37, s30, s16
	v_mov_b32_e32 v66, v63
	s_or_b32 s37, s37, 2
	v_mov_b32_e32 v67, v0
	s_cmp_lt_u32 s37, s14
	s_mov_b32 s37, 0
	s_cselect_b32 s38, -1, 0
	s_branch .LBB0_17
.LBB0_16:                               ;   in Loop: Header=BB0_17 Depth=3
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s39
	v_add_nc_u32_e32 v67, s3, v67
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v66, v[52:53]
	v_add_nc_u32_e32 v66, s26, v66
	v_cmp_lt_u32_e32 vcc_lo, 32, v67
	s_or_b32 s37, vcc_lo, s37
	s_andn2_b32 exec_lo, exec_lo, s37
	s_cbranch_execz .LBB0_19
.LBB0_17:                               ;   Parent Loop BB0_2 Depth=1
                                        ;     Parent Loop BB0_4 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	v_add_nc_u32_e32 v50, s2, v67
	v_mov_b32_e32 v52, 0
	v_mov_b32_e32 v53, 0
	v_cmp_gt_u32_e32 vcc_lo, s4, v50
	s_and_b32 s40, s38, vcc_lo
	s_and_saveexec_b32 s39, s40
	s_cbranch_execz .LBB0_16
; %bb.18:                               ;   in Loop: Header=BB0_17 Depth=3
	v_add_nc_u32_e32 v50, s33, v67
	v_lshlrev_b64 v[52:53], 3, v[50:51]
	v_add_co_u32 v52, vcc_lo, s8, v52
	v_add_co_ci_u32_e32 v53, vcc_lo, s9, v53, vcc_lo
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[52:53], v[52:53], off
	s_branch .LBB0_16
.LBB0_19:                               ; %Flow495
                                        ;   in Loop: Header=BB0_4 Depth=2
	s_or_b32 exec_lo, exec_lo, s36
	s_and_saveexec_b32 s36, s0
	s_cbranch_execz .LBB0_3
; %bb.20:                               ;   in Loop: Header=BB0_4 Depth=2
	s_or_b32 s37, s30, s16
	v_mov_b32_e32 v66, v62
	s_or_b32 s37, s37, 3
	v_mov_b32_e32 v67, v0
	s_cmp_lt_u32 s37, s14
	s_mov_b32 s37, 0
	s_cselect_b32 s38, -1, 0
	s_branch .LBB0_22
.LBB0_21:                               ;   in Loop: Header=BB0_22 Depth=3
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s39
	v_add_nc_u32_e32 v67, s3, v67
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v66, v[52:53]
	v_add_nc_u32_e32 v66, s26, v66
	v_cmp_lt_u32_e32 vcc_lo, 32, v67
	s_or_b32 s37, vcc_lo, s37
	s_andn2_b32 exec_lo, exec_lo, s37
	s_cbranch_execz .LBB0_3
.LBB0_22:                               ;   Parent Loop BB0_2 Depth=1
                                        ;     Parent Loop BB0_4 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	v_add_nc_u32_e32 v50, s2, v67
	v_mov_b32_e32 v52, 0
	v_mov_b32_e32 v53, 0
	v_cmp_gt_u32_e32 vcc_lo, s4, v50
	s_and_b32 s40, s38, vcc_lo
	s_and_saveexec_b32 s39, s40
	s_cbranch_execz .LBB0_21
; %bb.23:                               ;   in Loop: Header=BB0_22 Depth=3
	v_add_nc_u32_e32 v50, s31, v67
	v_lshlrev_b64 v[52:53], 3, v[50:51]
	v_add_co_u32 v52, vcc_lo, s8, v52
	v_add_co_ci_u32_e32 v53, vcc_lo, s9, v53, vcc_lo
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[52:53], v[52:53], off
	s_branch .LBB0_21
.LBB0_24:                               ;   in Loop: Header=BB0_2 Depth=1
	v_mov_b32_e32 v62, v56
	s_mul_i32 s30, s29, 0x42
	s_mov_b32 s31, 0
	s_mov_b32 s33, s27
	s_mov_b32 s34, s24
	s_mov_b32 s35, s20
	s_mov_b32 s36, s28
	s_branch .LBB0_26
.LBB0_25:                               ;   in Loop: Header=BB0_26 Depth=2
                                        ; implicit-def: $sgpr31
                                        ; implicit-def: $vgpr62
                                        ; implicit-def: $sgpr36
                                        ; implicit-def: $sgpr35
                                        ; implicit-def: $sgpr34
                                        ; implicit-def: $sgpr33
	s_cbranch_execnz .LBB0_46
.LBB0_26:                               ;   Parent Loop BB0_2 Depth=1
                                        ; =>  This Loop Header: Depth=2
                                        ;       Child Loop BB0_29 Depth 3
                                        ;       Child Loop BB0_33 Depth 3
                                        ;       Child Loop BB0_39 Depth 3
                                        ;       Child Loop BB0_43 Depth 3
	s_and_saveexec_b32 s37, s1
	s_cbranch_execz .LBB0_35
; %bb.27:                               ;   in Loop: Header=BB0_26 Depth=2
	s_lshl_b32 s38, s31, 1
	v_mov_b32_e32 v63, v62
	s_add_i32 s38, s38, s30
	v_mov_b32_e32 v64, v0
	s_cmp_lt_u32 s38, s17
	s_mov_b32 s39, 0
	s_cselect_b32 s40, -1, 0
	s_branch .LBB0_29
.LBB0_28:                               ;   in Loop: Header=BB0_29 Depth=3
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s41
	v_add_nc_u32_e32 v64, s3, v64
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v63, v[52:53]
	v_add_nc_u32_e32 v63, s26, v63
	v_cmp_lt_u32_e32 vcc_lo, 47, v64
	s_or_b32 s39, vcc_lo, s39
	s_andn2_b32 exec_lo, exec_lo, s39
	s_cbranch_execz .LBB0_31
.LBB0_29:                               ;   Parent Loop BB0_2 Depth=1
                                        ;     Parent Loop BB0_26 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	v_add_nc_u32_e32 v50, s6, v64
	v_mov_b32_e32 v52, 0
	v_mov_b32_e32 v53, 0
	v_cmp_gt_u32_e32 vcc_lo, s15, v50
	s_and_b32 s42, s40, vcc_lo
	s_and_saveexec_b32 s41, s42
	s_cbranch_execz .LBB0_28
; %bb.30:                               ;   in Loop: Header=BB0_29 Depth=3
	v_add_nc_u32_e32 v50, s36, v64
	v_lshlrev_b64 v[52:53], 3, v[50:51]
	v_add_co_u32 v52, vcc_lo, s10, v52
	v_add_co_ci_u32_e32 v53, vcc_lo, s11, v53, vcc_lo
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[52:53], v[52:53], off
	s_branch .LBB0_28
.LBB0_31:                               ;   in Loop: Header=BB0_26 Depth=2
	s_or_b32 exec_lo, exec_lo, s39
	s_or_b32 s39, s38, 1
	v_mov_b32_e32 v63, v0
	s_cmp_lt_u32 s39, s17
	s_movk_i32 s38, 0x180
	s_mov_b32 s39, 0
	s_cselect_b32 s40, -1, 0
	s_branch .LBB0_33
.LBB0_32:                               ;   in Loop: Header=BB0_33 Depth=3
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s41
	v_add_nc_u32_e32 v63, s3, v63
	v_add_nc_u32_e32 v50, s38, v62
	s_add_i32 s38, s38, s26
	v_cmp_lt_u32_e32 vcc_lo, 47, v63
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v50, v[52:53]
	s_or_b32 s39, vcc_lo, s39
	s_waitcnt_depctr 0xffe3
	s_andn2_b32 exec_lo, exec_lo, s39
	s_cbranch_execz .LBB0_35
.LBB0_33:                               ;   Parent Loop BB0_2 Depth=1
                                        ;     Parent Loop BB0_26 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	v_add_nc_u32_e32 v50, s6, v63
	v_mov_b32_e32 v52, 0
	v_mov_b32_e32 v53, 0
	v_cmp_gt_u32_e32 vcc_lo, s15, v50
	s_and_b32 s42, s40, vcc_lo
	s_and_saveexec_b32 s41, s42
	s_cbranch_execz .LBB0_32
; %bb.34:                               ;   in Loop: Header=BB0_33 Depth=3
	v_add_nc_u32_e32 v50, s35, v63
	v_lshlrev_b64 v[52:53], 3, v[50:51]
	v_add_co_u32 v52, vcc_lo, s10, v52
	v_add_co_ci_u32_e32 v53, vcc_lo, s11, v53, vcc_lo
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[52:53], v[52:53], off
	s_branch .LBB0_32
.LBB0_35:                               ; %Flow490
                                        ;   in Loop: Header=BB0_26 Depth=2
	s_or_b32 exec_lo, exec_lo, s37
	s_cmp_eq_u32 s31, 32
	s_cbranch_scc1 .LBB0_25
; %bb.36:                               ;   in Loop: Header=BB0_26 Depth=2
	s_and_saveexec_b32 s37, s1
	s_cbranch_execz .LBB0_45
; %bb.37:                               ;   in Loop: Header=BB0_26 Depth=2
	s_lshl_b32 s38, s31, 1
	v_mov_b32_e32 v63, v0
	s_add_i32 s38, s30, s38
	s_movk_i32 s39, 0x300
	s_add_i32 s38, s38, 2
	s_mov_b32 s40, 0
	s_cmp_lt_u32 s38, s17
	s_cselect_b32 s41, -1, 0
	s_branch .LBB0_39
.LBB0_38:                               ;   in Loop: Header=BB0_39 Depth=3
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s42
	v_add_nc_u32_e32 v63, s3, v63
	v_add_nc_u32_e32 v50, s39, v62
	s_add_i32 s39, s39, s26
	v_cmp_lt_u32_e32 vcc_lo, 47, v63
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v50, v[52:53]
	s_or_b32 s40, vcc_lo, s40
	s_waitcnt_depctr 0xffe3
	s_andn2_b32 exec_lo, exec_lo, s40
	s_cbranch_execz .LBB0_41
.LBB0_39:                               ;   Parent Loop BB0_2 Depth=1
                                        ;     Parent Loop BB0_26 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	v_add_nc_u32_e32 v50, s6, v63
	v_mov_b32_e32 v52, 0
	v_mov_b32_e32 v53, 0
	v_cmp_gt_u32_e32 vcc_lo, s15, v50
	s_and_b32 s43, s41, vcc_lo
	s_and_saveexec_b32 s42, s43
	s_cbranch_execz .LBB0_38
; %bb.40:                               ;   in Loop: Header=BB0_39 Depth=3
	v_add_nc_u32_e32 v50, s34, v63
	v_lshlrev_b64 v[52:53], 3, v[50:51]
	v_add_co_u32 v52, vcc_lo, s10, v52
	v_add_co_ci_u32_e32 v53, vcc_lo, s11, v53, vcc_lo
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[52:53], v[52:53], off
	s_branch .LBB0_38
.LBB0_41:                               ;   in Loop: Header=BB0_26 Depth=2
	s_or_b32 exec_lo, exec_lo, s40
	s_or_b32 s39, s38, 1
	v_mov_b32_e32 v63, v0
	s_cmp_lt_u32 s39, s17
	s_movk_i32 s38, 0x480
	s_mov_b32 s39, 0
	s_cselect_b32 s40, -1, 0
	s_branch .LBB0_43
.LBB0_42:                               ;   in Loop: Header=BB0_43 Depth=3
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s41
	v_add_nc_u32_e32 v63, s3, v63
	v_add_nc_u32_e32 v50, s38, v62
	s_add_i32 s38, s38, s26
	v_cmp_lt_u32_e32 vcc_lo, 47, v63
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v50, v[52:53]
	s_or_b32 s39, vcc_lo, s39
	s_waitcnt_depctr 0xffe3
	s_andn2_b32 exec_lo, exec_lo, s39
	s_cbranch_execz .LBB0_45
.LBB0_43:                               ;   Parent Loop BB0_2 Depth=1
                                        ;     Parent Loop BB0_26 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	v_add_nc_u32_e32 v50, s6, v63
	v_mov_b32_e32 v52, 0
	v_mov_b32_e32 v53, 0
	v_cmp_gt_u32_e32 vcc_lo, s15, v50
	s_and_b32 s42, s40, vcc_lo
	s_and_saveexec_b32 s41, s42
	s_cbranch_execz .LBB0_42
; %bb.44:                               ;   in Loop: Header=BB0_43 Depth=3
	v_add_nc_u32_e32 v50, s33, v63
	v_lshlrev_b64 v[52:53], 3, v[50:51]
	v_add_co_u32 v52, vcc_lo, s10, v52
	v_add_co_ci_u32_e32 v53, vcc_lo, s11, v53, vcc_lo
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[52:53], v[52:53], off
	s_branch .LBB0_42
.LBB0_45:                               ; %Flow488
                                        ;   in Loop: Header=BB0_26 Depth=2
	s_or_b32 exec_lo, exec_lo, s37
	s_add_i32 s31, s31, 2
	v_add_nc_u32_e32 v62, 0x600, v62
	s_add_i32 s36, s36, s19
	s_add_i32 s35, s35, s19
	s_add_i32 s34, s34, s19
	s_add_i32 s33, s33, s19
	s_branch .LBB0_26
.LBB0_46:                               ;   in Loop: Header=BB0_2 Depth=1
	v_mov_b32_e32 v50, v55
	s_mov_b32 s30, 0
	s_waitcnt lgkmcnt(0)
	; wave barrier
	s_waitcnt lgkmcnt(0)
.LBB0_47:                               ;   Parent Loop BB0_2 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_add_nc_u32_e32 v52, s30, v61
	ds_read2_b64 v[62:65], v50 offset0:2 offset1:48
	ds_read2_b64 v[66:69], v50 offset1:1
	ds_read2_b64 v[70:73], v50 offset0:49 offset1:50
	v_add_nc_u32_e32 v50, 0x300, v50
	s_add_i32 s30, s30, 8
	ds_read2_b64 v[74:77], v52 offset1:33
	ds_read2_b64 v[78:81], v52 offset0:66 offset1:99
	ds_read2_b64 v[82:85], v52 offset0:132 offset1:165
	ds_read2_b64 v[86:89], v52 offset0:198 offset1:231
	s_cmpk_lg_i32 s30, 0x108
	s_waitcnt lgkmcnt(3)
	v_fma_f32 v48, v74, v66, v48
	v_fmac_f32_e32 v49, v74, v67
	v_fma_f32 v46, v74, v68, v46
	v_fmac_f32_e32 v47, v74, v69
	v_fma_f32 v44, v74, v62, v44
	v_fmac_f32_e32 v45, v74, v63
	v_fma_f32 v42, v76, v66, v42
	v_fmac_f32_e32 v43, v76, v67
	v_fma_f32 v40, v76, v68, v40
	v_fmac_f32_e32 v41, v76, v69
	v_fma_f32 v38, v76, v62, v38
	v_fmac_f32_e32 v39, v76, v63
	s_waitcnt lgkmcnt(2)
	v_fma_f32 v36, v78, v66, v36
	v_fmac_f32_e32 v37, v78, v67
	v_fma_f32 v34, v78, v68, v34
	v_fmac_f32_e32 v35, v78, v69
	v_fma_f32 v32, v78, v62, v32
	v_fmac_f32_e32 v33, v78, v63
	v_fma_f32 v30, v80, v66, v30
	v_fmac_f32_e32 v31, v80, v67
	v_fma_f32 v28, v80, v68, v28
	v_fmac_f32_e32 v29, v80, v69
	v_fma_f32 v26, v80, v62, v26
	v_fmac_f32_e32 v27, v80, v63
	s_waitcnt lgkmcnt(1)
	v_fma_f32 v24, v82, v66, v24
	v_fmac_f32_e32 v25, v82, v67
	v_fma_f32 v22, v82, v68, v22
	v_fmac_f32_e32 v23, v82, v69
	v_fma_f32 v20, v82, v62, v20
	v_fmac_f32_e32 v21, v82, v63
	v_fma_f32 v18, v84, v66, v18
	v_fmac_f32_e32 v19, v84, v67
	v_fma_f32 v16, v84, v68, v16
	v_fmac_f32_e32 v17, v84, v69
	v_fma_f32 v14, v84, v62, v14
	v_fmac_f32_e32 v15, v84, v63
	s_waitcnt lgkmcnt(0)
	v_fma_f32 v12, v86, v66, v12
	v_fmac_f32_e32 v13, v86, v67
	v_fma_f32 v10, v86, v68, v10
	v_fmac_f32_e32 v11, v86, v69
	v_fma_f32 v8, v86, v62, v8
	v_fmac_f32_e32 v9, v86, v63
	v_fma_f32 v6, v88, v66, v6
	v_fmac_f32_e32 v7, v88, v67
	v_fma_f32 v4, v88, v68, v4
	v_fmac_f32_e32 v5, v88, v69
	v_fma_f32 v2, v88, v62, v2
	v_fmac_f32_e32 v3, v88, v63
	v_fmac_f32_e32 v49, v75, v65
	v_fmac_f32_e32 v48, v75, v64
	v_fmac_f32_e32 v47, v75, v71
	v_fmac_f32_e32 v46, v75, v70
	v_fmac_f32_e32 v45, v75, v73
	v_fmac_f32_e32 v44, v75, v72
	v_fmac_f32_e32 v43, v77, v65
	v_fmac_f32_e32 v42, v77, v64
	v_fmac_f32_e32 v41, v77, v71
	v_fmac_f32_e32 v40, v77, v70
	v_fmac_f32_e32 v39, v77, v73
	v_fmac_f32_e32 v38, v77, v72
	v_fmac_f32_e32 v37, v79, v65
	v_fmac_f32_e32 v36, v79, v64
	v_fmac_f32_e32 v35, v79, v71
	v_fmac_f32_e32 v34, v79, v70
	v_fmac_f32_e32 v33, v79, v73
	v_fmac_f32_e32 v32, v79, v72
	v_fmac_f32_e32 v31, v81, v65
	v_fmac_f32_e32 v30, v81, v64
	v_fmac_f32_e32 v29, v81, v71
	v_fmac_f32_e32 v28, v81, v70
	v_fmac_f32_e32 v27, v81, v73
	v_fmac_f32_e32 v26, v81, v72
	v_fmac_f32_e32 v25, v83, v65
	v_fmac_f32_e32 v24, v83, v64
	v_fmac_f32_e32 v23, v83, v71
	v_fmac_f32_e32 v22, v83, v70
	v_fmac_f32_e32 v21, v83, v73
	v_fmac_f32_e32 v20, v83, v72
	v_fmac_f32_e32 v19, v85, v65
	v_fmac_f32_e32 v18, v85, v64
	v_fmac_f32_e32 v17, v85, v71
	v_fmac_f32_e32 v16, v85, v70
	v_fmac_f32_e32 v15, v85, v73
	v_fmac_f32_e32 v14, v85, v72
	v_fmac_f32_e32 v13, v87, v65
	v_fmac_f32_e32 v12, v87, v64
	v_fmac_f32_e32 v11, v87, v71
	v_fmac_f32_e32 v10, v87, v70
	v_fmac_f32_e32 v9, v87, v73
	v_fmac_f32_e32 v8, v87, v72
	v_fmac_f32_e32 v7, v89, v65
	v_fmac_f32_e32 v6, v89, v64
	v_fmac_f32_e32 v5, v89, v71
	v_fmac_f32_e32 v4, v89, v70
	v_fmac_f32_e32 v3, v89, v73
	v_fmac_f32_e32 v2, v89, v72
	s_cbranch_scc1 .LBB0_47
; %bb.48:                               ;   in Loop: Header=BB0_2 Depth=1
	s_add_i32 s29, s29, 1
	s_add_i32 s25, s25, 33
	s_add_i32 s2, s2, 33
	s_add_i32 s21, s21, 33
	s_add_i32 s22, s22, 33
	s_add_i32 s23, s23, 33
	s_add_i32 s28, s28, s5
	s_add_i32 s20, s20, s5
	s_add_i32 s24, s24, s5
	s_add_i32 s27, s27, s5
	s_cmp_eq_u32 s29, s18
	s_waitcnt lgkmcnt(0)
	; wave barrier
	s_cbranch_scc0 .LBB0_2
.LBB0_49:                               ; %Flow500
	v_lshl_add_u32 v50, v1, 3, s16
	v_add_nc_u32_e32 v0, s6, v54
	v_mul_lo_u32 v51, s15, v50
	v_cmp_gt_u32_e64 s2, s14, v50
	v_cmp_gt_u32_e32 vcc_lo, s15, v0
	s_and_b32 s0, s2, vcc_lo
	s_and_saveexec_b32 s1, s0
	s_cbranch_execz .LBB0_51
; %bb.50:
	v_add_nc_u32_e32 v52, v51, v0
	v_mov_b32_e32 v53, 0
	v_lshlrev_b64 v[52:53], 3, v[52:53]
	v_add_co_u32 v52, s0, s12, v52
	v_add_co_ci_u32_e64 v53, s0, s13, v53, s0
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[52:53], v[48:49], off
.LBB0_51:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s1
	v_add_nc_u32_e32 v1, 1, v0
	v_cmp_gt_u32_e64 s0, s15, v1
	s_and_b32 s1, s2, s0
	s_and_saveexec_b32 s3, s1
	s_cbranch_execz .LBB0_53
; %bb.52:
	v_add_nc_u32_e32 v48, v51, v1
	v_mov_b32_e32 v49, 0
	v_lshlrev_b64 v[48:49], 3, v[48:49]
	v_add_co_u32 v48, s1, s12, v48
	v_add_co_ci_u32_e64 v49, s1, s13, v49, s1
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[48:49], v[46:47], off
.LBB0_53:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_add_nc_u32_e32 v46, 2, v0
	v_cmp_gt_u32_e64 s1, s15, v46
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execz .LBB0_55
; %bb.54:
	v_add_nc_u32_e32 v47, v51, v46
	v_mov_b32_e32 v48, 0
	v_lshlrev_b64 v[47:48], 3, v[47:48]
	v_add_co_u32 v47, s2, s12, v47
	v_add_co_ci_u32_e64 v48, s2, s13, v48, s2
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[47:48], v[44:45], off
.LBB0_55:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_or_b32_e32 v44, 1, v50
	v_cmp_gt_u32_e64 s2, s14, v44
	v_add_nc_u32_e32 v44, s15, v51
	s_and_b32 s3, s2, vcc_lo
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_83
; %bb.56:
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_84
.LBB0_57:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execz .LBB0_59
.LBB0_58:
	v_add_nc_u32_e32 v40, v44, v46
	v_mov_b32_e32 v41, 0
	v_lshlrev_b64 v[40:41], 3, v[40:41]
	v_add_co_u32 v40, s2, s12, v40
	v_add_co_ci_u32_e64 v41, s2, s13, v41, s2
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[40:41], v[38:39], off
.LBB0_59:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_or_b32_e32 v38, 2, v50
	v_cmp_gt_u32_e64 s2, s14, v38
	v_add_nc_u32_e32 v38, s15, v44
	s_and_b32 s3, s2, vcc_lo
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_85
; %bb.60:
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_86
.LBB0_61:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execz .LBB0_63
.LBB0_62:
	v_add_nc_u32_e32 v34, v38, v46
	v_mov_b32_e32 v35, 0
	v_lshlrev_b64 v[34:35], 3, v[34:35]
	v_add_co_u32 v34, s2, s12, v34
	v_add_co_ci_u32_e64 v35, s2, s13, v35, s2
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[34:35], v[32:33], off
.LBB0_63:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_or_b32_e32 v32, 3, v50
	v_cmp_gt_u32_e64 s2, s14, v32
	v_add_nc_u32_e32 v32, s15, v38
	s_and_b32 s3, s2, vcc_lo
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_87
; %bb.64:
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_88
.LBB0_65:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execz .LBB0_67
.LBB0_66:
	v_add_nc_u32_e32 v28, v32, v46
	v_mov_b32_e32 v29, 0
	v_lshlrev_b64 v[28:29], 3, v[28:29]
	v_add_co_u32 v28, s2, s12, v28
	v_add_co_ci_u32_e64 v29, s2, s13, v29, s2
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[28:29], v[26:27], off
.LBB0_67:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_or_b32_e32 v26, 4, v50
	v_cmp_gt_u32_e64 s2, s14, v26
	v_add_nc_u32_e32 v26, s15, v32
	s_and_b32 s3, s2, vcc_lo
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_89
; %bb.68:
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_90
.LBB0_69:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execz .LBB0_71
.LBB0_70:
	v_add_nc_u32_e32 v22, v26, v46
	v_mov_b32_e32 v23, 0
	v_lshlrev_b64 v[22:23], 3, v[22:23]
	v_add_co_u32 v22, s2, s12, v22
	v_add_co_ci_u32_e64 v23, s2, s13, v23, s2
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[22:23], v[20:21], off
.LBB0_71:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_or_b32_e32 v20, 5, v50
	v_cmp_gt_u32_e64 s2, s14, v20
	v_add_nc_u32_e32 v20, s15, v26
	s_and_b32 s3, s2, vcc_lo
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_91
; %bb.72:
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_92
.LBB0_73:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execz .LBB0_75
.LBB0_74:
	v_add_nc_u32_e32 v16, v20, v46
	v_mov_b32_e32 v17, 0
	v_lshlrev_b64 v[16:17], 3, v[16:17]
	v_add_co_u32 v16, s2, s12, v16
	v_add_co_ci_u32_e64 v17, s2, s13, v17, s2
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[16:17], v[14:15], off
.LBB0_75:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_or_b32_e32 v14, 6, v50
	v_cmp_gt_u32_e64 s2, s14, v14
	v_add_nc_u32_e32 v14, s15, v20
	s_and_b32 s3, s2, vcc_lo
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_93
; %bb.76:
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_94
.LBB0_77:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execz .LBB0_79
.LBB0_78:
	v_add_nc_u32_e32 v10, v14, v46
	v_mov_b32_e32 v11, 0
	v_lshlrev_b64 v[10:11], 3, v[10:11]
	v_add_co_u32 v10, s2, s12, v10
	v_add_co_ci_u32_e64 v11, s2, s13, v11, s2
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[10:11], v[8:9], off
.LBB0_79:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_or_b32_e32 v8, 7, v50
	v_cmp_gt_u32_e64 s2, s14, v8
	v_add_nc_u32_e32 v8, s15, v14
	s_and_b32 s4, s2, vcc_lo
	s_and_saveexec_b32 s3, s4
	s_cbranch_execnz .LBB0_95
; %bb.80:
	s_or_b32 exec_lo, exec_lo, s3
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s0, s3
	s_cbranch_execnz .LBB0_96
.LBB0_81:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s0
	s_and_b32 s0, s2, s1
	s_and_saveexec_b32 s1, s0
	s_cbranch_execnz .LBB0_97
.LBB0_82:
	s_endpgm
.LBB0_83:
	v_add_nc_u32_e32 v47, v44, v0
	v_mov_b32_e32 v48, 0
	v_lshlrev_b64 v[47:48], 3, v[47:48]
	v_add_co_u32 v47, s3, s12, v47
	v_add_co_ci_u32_e64 v48, s3, s13, v48, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[47:48], v[42:43], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execz .LBB0_57
.LBB0_84:
	v_add_nc_u32_e32 v42, v44, v1
	v_mov_b32_e32 v43, 0
	v_lshlrev_b64 v[42:43], 3, v[42:43]
	v_add_co_u32 v42, s3, s12, v42
	v_add_co_ci_u32_e64 v43, s3, s13, v43, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[42:43], v[40:41], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execnz .LBB0_58
	s_branch .LBB0_59
.LBB0_85:
	v_add_nc_u32_e32 v39, v38, v0
	v_mov_b32_e32 v40, 0
	v_lshlrev_b64 v[39:40], 3, v[39:40]
	v_add_co_u32 v39, s3, s12, v39
	v_add_co_ci_u32_e64 v40, s3, s13, v40, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[39:40], v[36:37], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execz .LBB0_61
.LBB0_86:
	v_add_nc_u32_e32 v36, v38, v1
	v_mov_b32_e32 v37, 0
	v_lshlrev_b64 v[36:37], 3, v[36:37]
	v_add_co_u32 v36, s3, s12, v36
	v_add_co_ci_u32_e64 v37, s3, s13, v37, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[36:37], v[34:35], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execnz .LBB0_62
	s_branch .LBB0_63
.LBB0_87:
	v_add_nc_u32_e32 v33, v32, v0
	v_mov_b32_e32 v34, 0
	v_lshlrev_b64 v[33:34], 3, v[33:34]
	v_add_co_u32 v33, s3, s12, v33
	v_add_co_ci_u32_e64 v34, s3, s13, v34, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[33:34], v[30:31], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execz .LBB0_65
.LBB0_88:
	v_add_nc_u32_e32 v30, v32, v1
	v_mov_b32_e32 v31, 0
	v_lshlrev_b64 v[30:31], 3, v[30:31]
	v_add_co_u32 v30, s3, s12, v30
	v_add_co_ci_u32_e64 v31, s3, s13, v31, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[30:31], v[28:29], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execnz .LBB0_66
	s_branch .LBB0_67
.LBB0_89:
	v_add_nc_u32_e32 v27, v26, v0
	v_mov_b32_e32 v28, 0
	v_lshlrev_b64 v[27:28], 3, v[27:28]
	v_add_co_u32 v27, s3, s12, v27
	v_add_co_ci_u32_e64 v28, s3, s13, v28, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[27:28], v[24:25], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execz .LBB0_69
.LBB0_90:
	v_add_nc_u32_e32 v24, v26, v1
	v_mov_b32_e32 v25, 0
	v_lshlrev_b64 v[24:25], 3, v[24:25]
	v_add_co_u32 v24, s3, s12, v24
	v_add_co_ci_u32_e64 v25, s3, s13, v25, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[24:25], v[22:23], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execnz .LBB0_70
	s_branch .LBB0_71
.LBB0_91:
	v_add_nc_u32_e32 v21, v20, v0
	v_mov_b32_e32 v22, 0
	v_lshlrev_b64 v[21:22], 3, v[21:22]
	v_add_co_u32 v21, s3, s12, v21
	v_add_co_ci_u32_e64 v22, s3, s13, v22, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[21:22], v[18:19], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execz .LBB0_73
.LBB0_92:
	v_add_nc_u32_e32 v18, v20, v1
	v_mov_b32_e32 v19, 0
	v_lshlrev_b64 v[18:19], 3, v[18:19]
	v_add_co_u32 v18, s3, s12, v18
	v_add_co_ci_u32_e64 v19, s3, s13, v19, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[18:19], v[16:17], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execnz .LBB0_74
	s_branch .LBB0_75
.LBB0_93:
	v_add_nc_u32_e32 v15, v14, v0
	v_mov_b32_e32 v16, 0
	v_lshlrev_b64 v[15:16], 3, v[15:16]
	v_add_co_u32 v15, s3, s12, v15
	v_add_co_ci_u32_e64 v16, s3, s13, v16, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[15:16], v[12:13], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execz .LBB0_77
.LBB0_94:
	v_add_nc_u32_e32 v12, v14, v1
	v_mov_b32_e32 v13, 0
	v_lshlrev_b64 v[12:13], 3, v[12:13]
	v_add_co_u32 v12, s3, s12, v12
	v_add_co_ci_u32_e64 v13, s3, s13, v13, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[12:13], v[10:11], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execnz .LBB0_78
	s_branch .LBB0_79
.LBB0_95:
	v_add_nc_u32_e32 v9, v8, v0
	v_mov_b32_e32 v10, 0
	v_lshlrev_b64 v[9:10], 3, v[9:10]
	v_add_co_u32 v9, vcc_lo, s12, v9
	v_add_co_ci_u32_e32 v10, vcc_lo, s13, v10, vcc_lo
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[9:10], v[6:7], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s0, s3
	s_cbranch_execz .LBB0_81
.LBB0_96:
	v_add_nc_u32_e32 v0, v8, v1
	v_mov_b32_e32 v1, 0
	v_lshlrev_b64 v[0:1], 3, v[0:1]
	v_add_co_u32 v0, vcc_lo, s12, v0
	v_add_co_ci_u32_e32 v1, vcc_lo, s13, v1, vcc_lo
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[0:1], v[4:5], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s0
	s_and_b32 s0, s2, s1
	s_and_saveexec_b32 s1, s0
	s_cbranch_execz .LBB0_82
.LBB0_97:
	v_add_nc_u32_e32 v0, v8, v46
	v_mov_b32_e32 v1, 0
	v_lshlrev_b64 v[0:1], 3, v[0:1]
	v_add_co_u32 v0, vcc_lo, s12, v0
	v_add_co_ci_u32_e32 v1, vcc_lo, s13, v1, vcc_lo
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[0:1], v[2:3], off
	s_endpgm
	.section	.rodata,"a",@progbits
	.p2align	6, 0x0
	.amdhsa_kernel mul
		.amdhsa_group_segment_fixed_size 29568
		.amdhsa_private_segment_fixed_size 0
		.amdhsa_kernarg_size 296
		.amdhsa_user_sgpr_count 6
		.amdhsa_user_sgpr_private_segment_buffer 1
		.amdhsa_user_sgpr_dispatch_ptr 0
		.amdhsa_user_sgpr_queue_ptr 0
		.amdhsa_user_sgpr_kernarg_segment_ptr 1
		.amdhsa_user_sgpr_dispatch_id 0
		.amdhsa_user_sgpr_flat_scratch_init 0
		.amdhsa_user_sgpr_private_segment_size 0
		.amdhsa_wavefront_size32 1
		.amdhsa_uses_dynamic_stack 0
		.amdhsa_system_sgpr_private_segment_wavefront_offset 0
		.amdhsa_system_sgpr_workgroup_id_x 1
		.amdhsa_system_sgpr_workgroup_id_y 1
		.amdhsa_system_sgpr_workgroup_id_z 0
		.amdhsa_system_sgpr_workgroup_info 0
		.amdhsa_system_vgpr_workitem_id 1
		.amdhsa_next_free_vgpr 90
		.amdhsa_next_free_sgpr 44
		.amdhsa_reserve_vcc 1
		.amdhsa_reserve_flat_scratch 0
		.amdhsa_reserve_xnack_mask 0
		.amdhsa_float_round_mode_32 0
		.amdhsa_float_round_mode_16_64 0
		.amdhsa_float_denorm_mode_32 3
		.amdhsa_float_denorm_mode_16_64 3
		.amdhsa_dx10_clamp 1
		.amdhsa_ieee_mode 1
		.amdhsa_fp16_overflow 0
		.amdhsa_workgroup_processor_mode 1
		.amdhsa_memory_ordered 1
		.amdhsa_forward_progress 0
		.amdhsa_shared_vgpr_count 0
		.amdhsa_exception_fp_ieee_invalid_op 0
		.amdhsa_exception_fp_denorm_src 0
		.amdhsa_exception_fp_ieee_div_zero 0
		.amdhsa_exception_fp_ieee_overflow 0
		.amdhsa_exception_fp_ieee_underflow 0
		.amdhsa_exception_fp_ieee_inexact 0
		.amdhsa_exception_int_div_zero 0
	.end_amdhsa_kernel
	.text
.Lfunc_end0:
	.size	mul, .Lfunc_end0-mul
                                        ; -- End function
	.section	.AMDGPU.csdata,"",@progbits
; Kernel info:
; codeLenInByte = 5112
; NumSgprs: 46
; NumVgprs: 90
; ScratchSize: 0
; MemoryBound: 0
; FloatMode: 240
; IeeeMode: 1
; LDSByteSize: 29568 bytes/workgroup (compile time only)
; SGPRBlocks: 5
; VGPRBlocks: 11
; NumSGPRsForWavesPerEU: 46
; NumVGPRsForWavesPerEU: 90
; Occupancy: 1
; WaveLimiterHint : 0
; COMPUTE_PGM_RSRC2:SCRATCH_EN: 0
; COMPUTE_PGM_RSRC2:USER_SGPR: 6
; COMPUTE_PGM_RSRC2:TRAP_HANDLER: 0
; COMPUTE_PGM_RSRC2:TGID_X_EN: 1
; COMPUTE_PGM_RSRC2:TGID_Y_EN: 1
; COMPUTE_PGM_RSRC2:TGID_Z_EN: 0
; COMPUTE_PGM_RSRC2:TIDIG_COMP_CNT: 1
	.text
	.p2alignl 6, 3214868480
	.fill 48, 4, 3214868480
	.ident	"clang version 20.0.0git (git@github.com:Compute-Mirrors/llvm-project 9dfb54abb25a32d9adf38f58b9a78d922f17b167)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.amdgpu_metadata
---
amdhsa.kernels:
  - .args:
      - .address_space:  global
        .name:           srcA
        .offset:         0
        .size:           8
        .type_name:      'MatrixType*'
        .value_kind:     global_buffer
      - .address_space:  global
        .name:           srcB
        .offset:         8
        .size:           8
        .type_name:      'MatrixType*'
        .value_kind:     global_buffer
      - .address_space:  global
        .name:           dstC
        .offset:         16
        .size:           8
        .type_name:      'MatrixType*'
        .value_kind:     global_buffer
      - .name:           dims
        .offset:         24
        .size:           16
        .type_name:      DeviceInfo
        .value_kind:     by_value
      - .offset:         40
        .size:           4
        .value_kind:     hidden_block_count_x
      - .offset:         44
        .size:           4
        .value_kind:     hidden_block_count_y
      - .offset:         48
        .size:           4
        .value_kind:     hidden_block_count_z
      - .offset:         52
        .size:           2
        .value_kind:     hidden_group_size_x
      - .offset:         54
        .size:           2
        .value_kind:     hidden_group_size_y
      - .offset:         56
        .size:           2
        .value_kind:     hidden_group_size_z
      - .offset:         58
        .size:           2
        .value_kind:     hidden_remainder_x
      - .offset:         60
        .size:           2
        .value_kind:     hidden_remainder_y
      - .offset:         62
        .size:           2
        .value_kind:     hidden_remainder_z
      - .offset:         80
        .size:           8
        .value_kind:     hidden_global_offset_x
      - .offset:         88
        .size:           8
        .value_kind:     hidden_global_offset_y
      - .offset:         96
        .size:           8
        .value_kind:     hidden_global_offset_z
      - .offset:         104
        .size:           2
        .value_kind:     hidden_grid_dims
    .group_segment_fixed_size: 29568
    .kernarg_segment_align: 8
    .kernarg_segment_size: 296
    .language:       OpenCL C
    .language_version:
      - 1
      - 2
    .max_flat_workgroup_size: 32
    .name:           mul
    .private_segment_fixed_size: 0
    .reqd_workgroup_size:
      - 16
      - 2
      - 1
    .sgpr_count:     46
    .sgpr_spill_count: 0
    .symbol:         mul.kd
    .uniform_work_group_size: 1
    .uses_dynamic_stack: false
    .vgpr_count:     90
    .vgpr_spill_count: 0
    .wavefront_size: 32
    .workgroup_processor_mode: 1
amdhsa.target:   'amdgcn-amd-amdhsa--gfx1010:xnack-'
amdhsa.version:
  - 1
  - 2
...

	.end_amdgpu_metadata
