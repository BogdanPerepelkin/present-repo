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
	s_load_dwordx8 s[8:15], s[4:5], 0x0
	s_load_dword s16, s[4:5], 0x20
	s_lshl_b32 s2, s7, 4
	s_lshl_b32 s3, s6, 7
	v_mov_b32_e32 v63, 0
	v_mov_b32_e32 v62, 0
	v_mov_b32_e32 v61, 0
	v_mov_b32_e32 v60, 0
	v_mov_b32_e32 v59, 0
	v_mov_b32_e32 v58, 0
	v_mov_b32_e32 v57, 0
	v_mov_b32_e32 v56, 0
	v_mov_b32_e32 v55, 0
	v_mov_b32_e32 v54, 0
	v_mov_b32_e32 v53, 0
	v_mov_b32_e32 v52, 0
	v_mov_b32_e32 v51, 0
	v_lshlrev_b32_e32 v66, 2, v1
	v_lshlrev_b32_e32 v67, 2, v0
	v_mov_b32_e32 v50, 0
	s_waitcnt lgkmcnt(0)
	s_lshr_b32 s6, s15, 1
	s_and_b32 s0, s16, 7
	s_lshr_b32 s1, s16, 3
	s_cmp_lg_u32 s0, 0
	v_mov_b32_e32 v49, 0
	s_cselect_b32 s0, -1, 0
	v_mov_b32_e32 v48, 0
	s_cmp_lg_u32 s0, 0
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
	v_mov_b32_e32 v32, 0
	s_addc_u32 s15, s1, 0
	s_cmp_eq_u32 s15, 0
	s_cbranch_scc1 .LBB0_58
; %bb.1:
	s_load_dword s0, s[4:5], 0x34
	s_lshr_b32 s4, s16, 1
	s_add_i32 s1, s2, 1
	v_lshlrev_b32_e32 v68, 3, v67
	s_mul_i32 s18, s4, s1
	v_lshl_add_u32 v69, v66, 5, 0x2000
	v_mov_b32_e32 v32, 0
	v_mov_b32_e32 v33, 0
	v_mov_b32_e32 v34, 0
	v_mov_b32_e32 v35, 0
	v_mov_b32_e32 v36, 0
	v_mov_b32_e32 v37, 0
	v_mov_b32_e32 v38, 0
	v_mov_b32_e32 v39, 0
	v_mov_b32_e32 v40, 0
	v_mov_b32_e32 v41, 0
	v_mov_b32_e32 v42, 0
	v_mov_b32_e32 v43, 0
	v_mov_b32_e32 v44, 0
	v_mov_b32_e32 v45, 0
	s_waitcnt lgkmcnt(0)
	s_and_b32 s1, s0, 0xffff
	s_lshr_b32 s23, s0, 16
	v_mad_u32_u24 v70, v1, s1, v0
	s_mul_i32 s23, s23, s1
	s_lshl_b32 s1, s6, 2
	v_mov_b32_e32 v46, 0
	v_mov_b32_e32 v47, 0
	v_lshlrev_b32_e32 v71, 3, v70
	v_cmp_gt_u32_e64 s0, 0x80, v70
	v_mov_b32_e32 v48, 0
	v_mov_b32_e32 v49, 0
	v_mov_b32_e32 v50, 0
	v_add_nc_u32_e32 v72, 0x2000, v71
	v_add_nc_u32_e32 v73, 0x2020, v71
	v_add_nc_u32_e32 v74, 0x2040, v71
	v_add_nc_u32_e32 v75, 0x2060, v71
	v_add_nc_u32_e32 v76, 0x400, v71
	v_add_nc_u32_e32 v77, 0x800, v71
	v_mov_b32_e32 v51, 0
	v_mov_b32_e32 v52, 0
	v_mov_b32_e32 v53, 0
	v_mov_b32_e32 v54, 0
	v_mov_b32_e32 v55, 0
	v_mov_b32_e32 v56, 0
	v_mov_b32_e32 v57, 0
	v_mov_b32_e32 v58, 0
	v_mov_b32_e32 v59, 0
	v_mov_b32_e32 v60, 0
	v_mov_b32_e32 v61, 0
	v_mov_b32_e32 v62, 0
	v_mov_b32_e32 v63, 0
	v_add_nc_u32_e32 v78, 0xc00, v71
	v_add_nc_u32_e32 v79, 0x1000, v71
	v_add_nc_u32_e32 v80, 0x1400, v71
	v_add_nc_u32_e32 v81, 0x1800, v71
	v_add_nc_u32_e32 v82, 0x1c00, v71
	v_mov_b32_e32 v65, 0
	s_add_i32 s19, s2, 2
	s_add_i32 s20, s2, 3
	s_lshl_b32 s21, s6, 1
	s_mul_i32 s22, s6, 3
	s_mul_i32 s24, s7, s4
	s_mul_i32 s27, s6, 5
	s_add_i32 s26, s3, s1
	s_mul_i32 s1, s6, 6
	s_mul_i32 s29, s6, 7
	s_lshl_b32 s5, s6, 3
	s_add_i32 s17, s6, s3
	s_lshl_b32 s7, s4, 2
	s_mul_i32 s19, s4, s19
	s_mul_i32 s20, s4, s20
	s_add_i32 s21, s3, s21
	s_add_i32 s22, s3, s22
	s_lshl_b32 s24, s24, 4
	s_lshl_b32 s25, s23, 3
	s_add_i32 s27, s3, s27
	s_add_i32 s28, s3, s1
	s_add_i32 s29, s3, s29
	s_mov_b32 s30, 0
	s_mov_b32 s31, s3
	s_mov_b32 s33, 0
	v_cmp_gt_u32_e32 vcc_lo, 4, v70
	s_branch .LBB0_3
.LBB0_2:                                ; %Flow482
                                        ;   in Loop: Header=BB0_3 Depth=1
	s_or_b32 exec_lo, exec_lo, s34
	s_waitcnt lgkmcnt(0)
	s_barrier
	ds_read2_b64 v[0:3], v68 offset1:1
	ds_read2_b64 v[4:7], v69 offset1:1
	ds_read2_b64 v[8:11], v68 offset0:2 offset1:3
	ds_read2_b64 v[12:15], v68 offset0:128 offset1:129
	ds_read2_b64 v[16:19], v68 offset0:130 offset1:131
	ds_read2_b64 v[20:23], v69 offset0:4 offset1:5
	ds_read2_b64 v[24:27], v69 offset0:8 offset1:9
	ds_read2_b64 v[28:31], v69 offset0:12 offset1:13
	s_add_i32 s33, s33, 1
	s_add_i32 s24, s24, 4
	s_add_i32 s30, s30, 4
	s_add_i32 s18, s18, 4
	s_add_i32 s19, s19, 4
	s_add_i32 s20, s20, 4
	s_add_i32 s31, s31, s5
	s_add_i32 s17, s17, s5
	s_add_i32 s21, s21, s5
	s_add_i32 s22, s22, s5
	s_add_i32 s26, s26, s5
	s_add_i32 s27, s27, s5
	s_add_i32 s28, s28, s5
	s_waitcnt lgkmcnt(6)
	v_fmac_f32_e32 v62, v4, v0
	s_waitcnt lgkmcnt(5)
	v_fmac_f32_e32 v59, v4, v9
	v_fmac_f32_e32 v58, v4, v8
	v_fmac_f32_e32 v57, v4, v11
	s_waitcnt lgkmcnt(2)
	v_fmac_f32_e32 v54, v20, v0
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v46, v24, v0
	s_waitcnt lgkmcnt(0)
	v_fmac_f32_e32 v38, v28, v0
	v_add_nc_u32_e32 v0, 0x800, v68
	v_fmac_f32_e32 v56, v4, v10
	v_fmac_f32_e32 v51, v20, v9
	v_fmac_f32_e32 v50, v20, v8
	v_fmac_f32_e32 v49, v20, v11
	v_fmac_f32_e32 v48, v20, v10
	v_fmac_f32_e32 v43, v24, v9
	v_fmac_f32_e32 v42, v24, v8
	v_fmac_f32_e32 v41, v24, v11
	v_fmac_f32_e32 v40, v24, v10
	v_fmac_f32_e32 v35, v28, v9
	v_fmac_f32_e32 v34, v28, v8
	v_fmac_f32_e32 v33, v28, v11
	ds_read2_b64 v[83:86], v0 offset1:1
	v_fmac_f32_e32 v32, v28, v10
	ds_read2_b64 v[8:11], v0 offset0:2 offset1:3
	ds_read2_b64 v[87:90], v0 offset0:128 offset1:129
	ds_read2_b64 v[91:94], v0 offset0:130 offset1:131
	v_fmac_f32_e32 v55, v20, v1
	v_fmac_f32_e32 v53, v20, v3
	v_fmac_f32_e32 v52, v20, v2
	v_fmac_f32_e32 v63, v4, v1
	v_fmac_f32_e32 v61, v4, v3
	v_fmac_f32_e32 v60, v4, v2
	v_fmac_f32_e32 v54, v21, v12
	v_fmac_f32_e32 v55, v21, v13
	v_fmac_f32_e32 v52, v21, v14
	v_fmac_f32_e32 v53, v21, v15
	v_fmac_f32_e32 v50, v21, v16
	v_fmac_f32_e32 v51, v21, v17
	v_fmac_f32_e32 v48, v21, v18
	v_fmac_f32_e32 v49, v21, v19
	v_fmac_f32_e32 v47, v24, v1
	v_fmac_f32_e32 v45, v24, v3
	v_fmac_f32_e32 v44, v24, v2
	v_fmac_f32_e32 v39, v28, v1
	v_fmac_f32_e32 v37, v28, v3
	v_fmac_f32_e32 v36, v28, v2
	v_fmac_f32_e32 v62, v5, v12
	v_fmac_f32_e32 v63, v5, v13
	v_fmac_f32_e32 v60, v5, v14
	v_fmac_f32_e32 v61, v5, v15
	v_fmac_f32_e32 v58, v5, v16
	v_fmac_f32_e32 v59, v5, v17
	v_fmac_f32_e32 v56, v5, v18
	v_fmac_f32_e32 v57, v5, v19
	v_fmac_f32_e32 v46, v25, v12
	v_fmac_f32_e32 v47, v25, v13
	v_fmac_f32_e32 v44, v25, v14
	v_fmac_f32_e32 v45, v25, v15
	v_fmac_f32_e32 v42, v25, v16
	v_fmac_f32_e32 v43, v25, v17
	v_fmac_f32_e32 v40, v25, v18
	v_fmac_f32_e32 v41, v25, v19
	v_fmac_f32_e32 v38, v29, v12
	v_fmac_f32_e32 v39, v29, v13
	v_fmac_f32_e32 v36, v29, v14
	v_fmac_f32_e32 v37, v29, v15
	v_fmac_f32_e32 v34, v29, v16
	v_fmac_f32_e32 v35, v29, v17
	v_fmac_f32_e32 v32, v29, v18
	v_fmac_f32_e32 v33, v29, v19
	s_waitcnt lgkmcnt(3)
	v_fmac_f32_e32 v55, v22, v84
	v_fmac_f32_e32 v54, v22, v83
	v_fmac_f32_e32 v53, v22, v86
	v_fmac_f32_e32 v52, v22, v85
	s_waitcnt lgkmcnt(2)
	v_fmac_f32_e32 v51, v22, v9
	v_fmac_f32_e32 v50, v22, v8
	v_fmac_f32_e32 v49, v22, v11
	v_fmac_f32_e32 v48, v22, v10
	v_add_nc_u32_e32 v4, 0x1000, v68
	v_fmac_f32_e32 v63, v6, v84
	v_fmac_f32_e32 v62, v6, v83
	v_fmac_f32_e32 v61, v6, v86
	v_fmac_f32_e32 v60, v6, v85
	v_fmac_f32_e32 v59, v6, v9
	v_fmac_f32_e32 v58, v6, v8
	v_fmac_f32_e32 v57, v6, v11
	v_fmac_f32_e32 v56, v6, v10
	ds_read2_b64 v[0:3], v69 offset0:2 offset1:3
	s_waitcnt lgkmcnt(2)
	v_fmac_f32_e32 v54, v23, v87
	v_fmac_f32_e32 v55, v23, v88
	v_fmac_f32_e32 v52, v23, v89
	v_fmac_f32_e32 v53, v23, v90
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v50, v23, v91
	v_fmac_f32_e32 v51, v23, v92
	v_fmac_f32_e32 v48, v23, v93
	v_fmac_f32_e32 v49, v23, v94
	v_fmac_f32_e32 v47, v26, v84
	v_fmac_f32_e32 v46, v26, v83
	v_fmac_f32_e32 v45, v26, v86
	v_fmac_f32_e32 v44, v26, v85
	v_fmac_f32_e32 v43, v26, v9
	v_fmac_f32_e32 v42, v26, v8
	v_fmac_f32_e32 v41, v26, v11
	v_fmac_f32_e32 v40, v26, v10
	v_fmac_f32_e32 v39, v30, v84
	ds_read2_b64 v[20:23], v4 offset1:1
	ds_read2_b64 v[16:19], v4 offset0:2 offset1:3
	v_fmac_f32_e32 v38, v30, v83
	v_fmac_f32_e32 v37, v30, v86
	v_fmac_f32_e32 v36, v30, v85
	v_fmac_f32_e32 v35, v30, v9
	v_fmac_f32_e32 v34, v30, v8
	v_fmac_f32_e32 v33, v30, v11
	v_fmac_f32_e32 v32, v30, v10
	v_fmac_f32_e32 v62, v7, v87
	v_fmac_f32_e32 v63, v7, v88
	v_fmac_f32_e32 v60, v7, v89
	v_fmac_f32_e32 v61, v7, v90
	v_fmac_f32_e32 v58, v7, v91
	v_fmac_f32_e32 v59, v7, v92
	v_fmac_f32_e32 v56, v7, v93
	v_fmac_f32_e32 v57, v7, v94
	v_fmac_f32_e32 v46, v27, v87
	v_fmac_f32_e32 v47, v27, v88
	v_fmac_f32_e32 v44, v27, v89
	v_fmac_f32_e32 v45, v27, v90
	v_fmac_f32_e32 v42, v27, v91
	v_fmac_f32_e32 v43, v27, v92
	v_fmac_f32_e32 v40, v27, v93
	v_fmac_f32_e32 v41, v27, v94
	ds_read2_b64 v[12:15], v4 offset0:128 offset1:129
	ds_read2_b64 v[8:11], v4 offset0:130 offset1:131
	v_fmac_f32_e32 v38, v31, v87
	v_fmac_f32_e32 v39, v31, v88
	v_fmac_f32_e32 v36, v31, v89
	v_fmac_f32_e32 v37, v31, v90
	ds_read2_b64 v[4:7], v69 offset0:6 offset1:7
	v_fmac_f32_e32 v34, v31, v91
	v_fmac_f32_e32 v35, v31, v92
	v_fmac_f32_e32 v32, v31, v93
	v_fmac_f32_e32 v33, v31, v94
	ds_read2_b64 v[24:27], v69 offset0:10 offset1:11
	ds_read2_b64 v[28:31], v69 offset0:14 offset1:15
	s_waitcnt lgkmcnt(6)
	v_fmac_f32_e32 v63, v0, v21
	v_fmac_f32_e32 v62, v0, v20
	v_fmac_f32_e32 v61, v0, v23
	v_fmac_f32_e32 v60, v0, v22
	s_waitcnt lgkmcnt(5)
	v_fmac_f32_e32 v59, v0, v17
	v_fmac_f32_e32 v58, v0, v16
	v_fmac_f32_e32 v57, v0, v19
	v_fmac_f32_e32 v56, v0, v18
	v_add_nc_u32_e32 v0, 0x1800, v68
	s_waitcnt lgkmcnt(4)
	v_fmac_f32_e32 v62, v1, v12
	v_fmac_f32_e32 v63, v1, v13
	v_fmac_f32_e32 v60, v1, v14
	v_fmac_f32_e32 v61, v1, v15
	s_waitcnt lgkmcnt(2)
	v_fmac_f32_e32 v55, v4, v21
	v_fmac_f32_e32 v54, v4, v20
	v_fmac_f32_e32 v53, v4, v23
	v_fmac_f32_e32 v52, v4, v22
	v_fmac_f32_e32 v51, v4, v17
	v_fmac_f32_e32 v50, v4, v16
	v_fmac_f32_e32 v49, v4, v19
	v_fmac_f32_e32 v48, v4, v18
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v47, v24, v21
	v_fmac_f32_e32 v46, v24, v20
	v_fmac_f32_e32 v45, v24, v23
	v_fmac_f32_e32 v44, v24, v22
	v_fmac_f32_e32 v43, v24, v17
	v_fmac_f32_e32 v42, v24, v16
	v_fmac_f32_e32 v41, v24, v19
	v_fmac_f32_e32 v40, v24, v18
	s_waitcnt lgkmcnt(0)
	v_fmac_f32_e32 v39, v28, v21
	v_fmac_f32_e32 v38, v28, v20
	v_fmac_f32_e32 v37, v28, v23
	v_fmac_f32_e32 v36, v28, v22
	v_fmac_f32_e32 v35, v28, v17
	v_fmac_f32_e32 v34, v28, v16
	v_fmac_f32_e32 v33, v28, v19
	ds_read2_b64 v[19:22], v0 offset1:1
	ds_read2_b64 v[83:86], v0 offset0:2 offset1:3
	v_fmac_f32_e32 v32, v28, v18
	v_fmac_f32_e32 v58, v1, v8
	v_fmac_f32_e32 v59, v1, v9
	v_fmac_f32_e32 v56, v1, v10
	v_fmac_f32_e32 v57, v1, v11
	v_fmac_f32_e32 v54, v5, v12
	v_fmac_f32_e32 v55, v5, v13
	v_fmac_f32_e32 v52, v5, v14
	v_fmac_f32_e32 v53, v5, v15
	v_fmac_f32_e32 v50, v5, v8
	v_fmac_f32_e32 v51, v5, v9
	v_fmac_f32_e32 v48, v5, v10
	v_fmac_f32_e32 v49, v5, v11
	v_fmac_f32_e32 v46, v25, v12
	v_fmac_f32_e32 v47, v25, v13
	v_fmac_f32_e32 v44, v25, v14
	v_fmac_f32_e32 v45, v25, v15
	v_fmac_f32_e32 v42, v25, v8
	v_fmac_f32_e32 v43, v25, v9
	v_fmac_f32_e32 v40, v25, v10
	v_fmac_f32_e32 v41, v25, v11
	v_fmac_f32_e32 v38, v29, v12
	v_fmac_f32_e32 v39, v29, v13
	v_fmac_f32_e32 v36, v29, v14
	v_fmac_f32_e32 v37, v29, v15
	v_fmac_f32_e32 v34, v29, v8
	ds_read2_b64 v[12:15], v0 offset0:128 offset1:129
	v_fmac_f32_e32 v35, v29, v9
	v_fmac_f32_e32 v32, v29, v10
	v_fmac_f32_e32 v33, v29, v11
	ds_read2_b64 v[8:11], v0 offset0:130 offset1:131
	s_waitcnt lgkmcnt(3)
	v_fmac_f32_e32 v63, v2, v20
	v_fmac_f32_e32 v62, v2, v19
	v_fmac_f32_e32 v61, v2, v22
	v_fmac_f32_e32 v60, v2, v21
	s_waitcnt lgkmcnt(2)
	v_fmac_f32_e32 v59, v2, v84
	v_fmac_f32_e32 v58, v2, v83
	v_fmac_f32_e32 v57, v2, v86
	v_fmac_f32_e32 v56, v2, v85
	v_fmac_f32_e32 v55, v6, v20
	v_fmac_f32_e32 v54, v6, v19
	v_fmac_f32_e32 v53, v6, v22
	v_fmac_f32_e32 v52, v6, v21
	v_fmac_f32_e32 v51, v6, v84
	v_fmac_f32_e32 v50, v6, v83
	v_fmac_f32_e32 v49, v6, v86
	v_fmac_f32_e32 v48, v6, v85
	v_fmac_f32_e32 v47, v26, v20
	v_fmac_f32_e32 v46, v26, v19
	v_fmac_f32_e32 v45, v26, v22
	v_fmac_f32_e32 v44, v26, v21
	v_fmac_f32_e32 v43, v26, v84
	v_fmac_f32_e32 v42, v26, v83
	v_fmac_f32_e32 v41, v26, v86
	v_fmac_f32_e32 v40, v26, v85
	v_fmac_f32_e32 v39, v30, v20
	v_fmac_f32_e32 v38, v30, v19
	v_fmac_f32_e32 v37, v30, v22
	v_fmac_f32_e32 v36, v30, v21
	v_fmac_f32_e32 v35, v30, v84
	v_fmac_f32_e32 v34, v30, v83
	v_fmac_f32_e32 v33, v30, v86
	v_fmac_f32_e32 v32, v30, v85
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v62, v3, v12
	v_fmac_f32_e32 v63, v3, v13
	v_fmac_f32_e32 v60, v3, v14
	v_fmac_f32_e32 v61, v3, v15
	s_waitcnt lgkmcnt(0)
	v_fmac_f32_e32 v58, v3, v8
	v_fmac_f32_e32 v59, v3, v9
	v_fmac_f32_e32 v56, v3, v10
	v_fmac_f32_e32 v57, v3, v11
	v_fmac_f32_e32 v54, v7, v12
	v_fmac_f32_e32 v55, v7, v13
	v_fmac_f32_e32 v52, v7, v14
	v_fmac_f32_e32 v53, v7, v15
	v_fmac_f32_e32 v50, v7, v8
	v_fmac_f32_e32 v51, v7, v9
	v_fmac_f32_e32 v48, v7, v10
	v_fmac_f32_e32 v49, v7, v11
	v_fmac_f32_e32 v46, v27, v12
	v_fmac_f32_e32 v47, v27, v13
	v_fmac_f32_e32 v44, v27, v14
	v_fmac_f32_e32 v45, v27, v15
	v_fmac_f32_e32 v42, v27, v8
	v_fmac_f32_e32 v43, v27, v9
	v_fmac_f32_e32 v40, v27, v10
	v_fmac_f32_e32 v41, v27, v11
	v_fmac_f32_e32 v38, v31, v12
	v_fmac_f32_e32 v39, v31, v13
	v_fmac_f32_e32 v36, v31, v14
	v_fmac_f32_e32 v37, v31, v15
	v_fmac_f32_e32 v34, v31, v8
	v_fmac_f32_e32 v35, v31, v9
	v_fmac_f32_e32 v32, v31, v10
	v_fmac_f32_e32 v33, v31, v11
	s_add_i32 s29, s29, s5
	s_cmp_eq_u32 s33, s15
	s_barrier
	s_cbranch_scc1 .LBB0_58
.LBB0_3:                                ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB0_5 Depth 2
                                        ;       Child Loop BB0_8 Depth 3
                                        ;       Child Loop BB0_13 Depth 3
                                        ;       Child Loop BB0_18 Depth 3
                                        ;       Child Loop BB0_23 Depth 3
                                        ;     Child Loop BB0_28 Depth 2
                                        ;     Child Loop BB0_32 Depth 2
                                        ;     Child Loop BB0_36 Depth 2
                                        ;     Child Loop BB0_40 Depth 2
                                        ;     Child Loop BB0_44 Depth 2
                                        ;     Child Loop BB0_48 Depth 2
                                        ;     Child Loop BB0_52 Depth 2
                                        ;     Child Loop BB0_56 Depth 2
	v_mov_b32_e32 v2, v75
	v_mov_b32_e32 v3, v74
	v_mov_b32_e32 v4, v73
	v_mov_b32_e32 v5, v72
	s_mov_b32 s34, s20
	s_mov_b32 s35, s19
	s_mov_b32 s36, s18
	s_mov_b32 s37, s24
	s_mov_b32 s38, 0
	s_branch .LBB0_5
.LBB0_4:                                ; %Flow484
                                        ;   in Loop: Header=BB0_5 Depth=2
	s_or_b32 exec_lo, exec_lo, s39
	v_add_nc_u32_e32 v5, 0x80, v5
	v_add_nc_u32_e32 v4, 0x80, v4
	v_add_nc_u32_e32 v3, 0x80, v3
	v_add_nc_u32_e32 v2, 0x80, v2
	s_add_i32 s38, s38, 4
	s_add_i32 s37, s37, s7
	s_add_i32 s36, s36, s7
	s_add_i32 s35, s35, s7
	s_add_i32 s34, s34, s7
	s_cmp_lg_u32 s38, 16
	s_cbranch_scc0 .LBB0_25
.LBB0_5:                                ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Loop Header: Depth=2
                                        ;       Child Loop BB0_8 Depth 3
                                        ;       Child Loop BB0_13 Depth 3
                                        ;       Child Loop BB0_18 Depth 3
                                        ;       Child Loop BB0_23 Depth 3
	s_and_saveexec_b32 s39, vcc_lo
	s_cbranch_execz .LBB0_10
; %bb.6:                                ;   in Loop: Header=BB0_5 Depth=2
	s_or_b32 s1, s38, s2
	v_mov_b32_e32 v6, v5
	v_mov_b32_e32 v7, v70
	s_cmp_lt_u32 s1, s14
	s_mov_b32 s40, 0
	s_cselect_b32 s41, -1, 0
	s_branch .LBB0_8
.LBB0_7:                                ;   in Loop: Header=BB0_8 Depth=3
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s42
	v_add_nc_u32_e32 v7, s23, v7
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v6, v[0:1]
	v_add_nc_u32_e32 v6, s25, v6
	v_cmp_lt_u32_e64 s1, 3, v7
	s_or_b32 s40, s1, s40
	s_andn2_b32 exec_lo, exec_lo, s40
	s_cbranch_execz .LBB0_10
.LBB0_8:                                ;   Parent Loop BB0_3 Depth=1
                                        ;     Parent Loop BB0_5 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	v_add_nc_u32_e32 v0, s30, v7
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s1, s4, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s1, s41, s1
	s_and_saveexec_b32 s42, s1
	s_cbranch_execz .LBB0_7
; %bb.9:                                ;   in Loop: Header=BB0_8 Depth=3
	v_add_nc_u32_e32 v64, s37, v7
	v_lshlrev_b64 v[0:1], 3, v[64:65]
	v_add_co_u32 v0, s1, s8, v0
	v_add_co_ci_u32_e64 v1, s1, s9, v1, s1
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_7
.LBB0_10:                               ; %Flow490
                                        ;   in Loop: Header=BB0_5 Depth=2
	s_or_b32 exec_lo, exec_lo, s39
	s_and_saveexec_b32 s39, vcc_lo
	s_cbranch_execz .LBB0_15
; %bb.11:                               ;   in Loop: Header=BB0_5 Depth=2
	s_or_b32 s1, s38, s2
	v_mov_b32_e32 v6, v4
	s_or_b32 s1, s1, 1
	v_mov_b32_e32 v7, v70
	s_cmp_lt_u32 s1, s14
	s_mov_b32 s40, 0
	s_cselect_b32 s41, -1, 0
	s_branch .LBB0_13
.LBB0_12:                               ;   in Loop: Header=BB0_13 Depth=3
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s42
	v_add_nc_u32_e32 v7, s23, v7
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v6, v[0:1]
	v_add_nc_u32_e32 v6, s25, v6
	v_cmp_lt_u32_e64 s1, 3, v7
	s_or_b32 s40, s1, s40
	s_andn2_b32 exec_lo, exec_lo, s40
	s_cbranch_execz .LBB0_15
.LBB0_13:                               ;   Parent Loop BB0_3 Depth=1
                                        ;     Parent Loop BB0_5 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	v_add_nc_u32_e32 v0, s30, v7
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s1, s4, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s1, s41, s1
	s_and_saveexec_b32 s42, s1
	s_cbranch_execz .LBB0_12
; %bb.14:                               ;   in Loop: Header=BB0_13 Depth=3
	v_add_nc_u32_e32 v64, s36, v7
	v_lshlrev_b64 v[0:1], 3, v[64:65]
	v_add_co_u32 v0, s1, s8, v0
	v_add_co_ci_u32_e64 v1, s1, s9, v1, s1
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_12
.LBB0_15:                               ; %Flow488
                                        ;   in Loop: Header=BB0_5 Depth=2
	s_or_b32 exec_lo, exec_lo, s39
	s_and_saveexec_b32 s39, vcc_lo
	s_cbranch_execz .LBB0_20
; %bb.16:                               ;   in Loop: Header=BB0_5 Depth=2
	s_or_b32 s1, s38, s2
	v_mov_b32_e32 v6, v3
	s_or_b32 s1, s1, 2
	v_mov_b32_e32 v7, v70
	s_cmp_lt_u32 s1, s14
	s_mov_b32 s40, 0
	s_cselect_b32 s41, -1, 0
	s_branch .LBB0_18
.LBB0_17:                               ;   in Loop: Header=BB0_18 Depth=3
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s42
	v_add_nc_u32_e32 v7, s23, v7
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v6, v[0:1]
	v_add_nc_u32_e32 v6, s25, v6
	v_cmp_lt_u32_e64 s1, 3, v7
	s_or_b32 s40, s1, s40
	s_andn2_b32 exec_lo, exec_lo, s40
	s_cbranch_execz .LBB0_20
.LBB0_18:                               ;   Parent Loop BB0_3 Depth=1
                                        ;     Parent Loop BB0_5 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	v_add_nc_u32_e32 v0, s30, v7
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s1, s4, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s1, s41, s1
	s_and_saveexec_b32 s42, s1
	s_cbranch_execz .LBB0_17
; %bb.19:                               ;   in Loop: Header=BB0_18 Depth=3
	v_add_nc_u32_e32 v64, s35, v7
	v_lshlrev_b64 v[0:1], 3, v[64:65]
	v_add_co_u32 v0, s1, s8, v0
	v_add_co_ci_u32_e64 v1, s1, s9, v1, s1
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_17
.LBB0_20:                               ; %Flow486
                                        ;   in Loop: Header=BB0_5 Depth=2
	s_or_b32 exec_lo, exec_lo, s39
	s_and_saveexec_b32 s39, vcc_lo
	s_cbranch_execz .LBB0_4
; %bb.21:                               ;   in Loop: Header=BB0_5 Depth=2
	s_or_b32 s1, s38, s2
	v_mov_b32_e32 v6, v2
	s_or_b32 s1, s1, 3
	v_mov_b32_e32 v7, v70
	s_cmp_lt_u32 s1, s14
	s_mov_b32 s40, 0
	s_cselect_b32 s41, -1, 0
	s_branch .LBB0_23
.LBB0_22:                               ;   in Loop: Header=BB0_23 Depth=3
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s42
	v_add_nc_u32_e32 v7, s23, v7
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v6, v[0:1]
	v_add_nc_u32_e32 v6, s25, v6
	v_cmp_lt_u32_e64 s1, 3, v7
	s_or_b32 s40, s1, s40
	s_andn2_b32 exec_lo, exec_lo, s40
	s_cbranch_execz .LBB0_4
.LBB0_23:                               ;   Parent Loop BB0_3 Depth=1
                                        ;     Parent Loop BB0_5 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	v_add_nc_u32_e32 v0, s30, v7
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s1, s4, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s1, s41, s1
	s_and_saveexec_b32 s42, s1
	s_cbranch_execz .LBB0_22
; %bb.24:                               ;   in Loop: Header=BB0_23 Depth=3
	v_add_nc_u32_e32 v64, s34, v7
	v_lshlrev_b64 v[0:1], 3, v[64:65]
	v_add_co_u32 v0, s1, s8, v0
	v_add_co_ci_u32_e64 v1, s1, s9, v1, s1
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_22
.LBB0_25:                               ;   in Loop: Header=BB0_3 Depth=1
	s_and_saveexec_b32 s34, s0
	s_cbranch_execz .LBB0_2
; %bb.26:                               ;   in Loop: Header=BB0_3 Depth=1
	s_lshl_b32 s35, s33, 3
	v_mov_b32_e32 v2, v71
	v_mov_b32_e32 v3, v70
	s_cmp_lt_u32 s35, s16
	s_mov_b32 s36, 0
	s_cselect_b32 s37, -1, 0
	s_branch .LBB0_28
.LBB0_27:                               ;   in Loop: Header=BB0_28 Depth=2
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s38
	v_add_nc_u32_e32 v3, s23, v3
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v2, v[0:1]
	v_add_nc_u32_e32 v2, s25, v2
	v_cmp_lt_u32_e64 s1, 0x7f, v3
	s_or_b32 s36, s1, s36
	s_andn2_b32 exec_lo, exec_lo, s36
	s_cbranch_execz .LBB0_30
.LBB0_28:                               ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_add_nc_u32_e32 v0, s3, v3
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s1, s6, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s1, s37, s1
	s_and_saveexec_b32 s38, s1
	s_cbranch_execz .LBB0_27
; %bb.29:                               ;   in Loop: Header=BB0_28 Depth=2
	v_add_nc_u32_e32 v64, s31, v3
	v_lshlrev_b64 v[0:1], 3, v[64:65]
	v_add_co_u32 v0, s1, s10, v0
	v_add_co_ci_u32_e64 v1, s1, s11, v1, s1
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_27
.LBB0_30:                               ;   in Loop: Header=BB0_3 Depth=1
	s_or_b32 exec_lo, exec_lo, s36
	s_or_b32 s1, s35, 1
	v_mov_b32_e32 v2, v76
	v_mov_b32_e32 v3, v70
	s_cmp_lt_u32 s1, s16
	s_mov_b32 s36, 0
	s_cselect_b32 s37, -1, 0
	s_branch .LBB0_32
.LBB0_31:                               ;   in Loop: Header=BB0_32 Depth=2
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s38
	v_add_nc_u32_e32 v3, s23, v3
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v2, v[0:1]
	v_add_nc_u32_e32 v2, s25, v2
	v_cmp_lt_u32_e64 s1, 0x7f, v3
	s_or_b32 s36, s1, s36
	s_andn2_b32 exec_lo, exec_lo, s36
	s_cbranch_execz .LBB0_34
.LBB0_32:                               ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_add_nc_u32_e32 v0, s3, v3
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s1, s6, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s1, s37, s1
	s_and_saveexec_b32 s38, s1
	s_cbranch_execz .LBB0_31
; %bb.33:                               ;   in Loop: Header=BB0_32 Depth=2
	v_add_nc_u32_e32 v64, s17, v3
	v_lshlrev_b64 v[0:1], 3, v[64:65]
	v_add_co_u32 v0, s1, s10, v0
	v_add_co_ci_u32_e64 v1, s1, s11, v1, s1
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_31
.LBB0_34:                               ;   in Loop: Header=BB0_3 Depth=1
	s_or_b32 exec_lo, exec_lo, s36
	s_or_b32 s1, s35, 2
	v_mov_b32_e32 v2, v77
	v_mov_b32_e32 v3, v70
	s_cmp_lt_u32 s1, s16
	s_mov_b32 s36, 0
	s_cselect_b32 s37, -1, 0
	s_branch .LBB0_36
.LBB0_35:                               ;   in Loop: Header=BB0_36 Depth=2
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s38
	v_add_nc_u32_e32 v3, s23, v3
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v2, v[0:1]
	v_add_nc_u32_e32 v2, s25, v2
	v_cmp_lt_u32_e64 s1, 0x7f, v3
	s_or_b32 s36, s1, s36
	s_andn2_b32 exec_lo, exec_lo, s36
	s_cbranch_execz .LBB0_38
.LBB0_36:                               ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_add_nc_u32_e32 v0, s3, v3
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s1, s6, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s1, s37, s1
	s_and_saveexec_b32 s38, s1
	s_cbranch_execz .LBB0_35
; %bb.37:                               ;   in Loop: Header=BB0_36 Depth=2
	v_add_nc_u32_e32 v64, s21, v3
	v_lshlrev_b64 v[0:1], 3, v[64:65]
	v_add_co_u32 v0, s1, s10, v0
	v_add_co_ci_u32_e64 v1, s1, s11, v1, s1
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_35
.LBB0_38:                               ;   in Loop: Header=BB0_3 Depth=1
	s_or_b32 exec_lo, exec_lo, s36
	s_or_b32 s1, s35, 3
	v_mov_b32_e32 v2, v78
	v_mov_b32_e32 v3, v70
	s_cmp_lt_u32 s1, s16
	s_mov_b32 s36, 0
	s_cselect_b32 s37, -1, 0
	s_branch .LBB0_40
.LBB0_39:                               ;   in Loop: Header=BB0_40 Depth=2
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s38
	v_add_nc_u32_e32 v3, s23, v3
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v2, v[0:1]
	v_add_nc_u32_e32 v2, s25, v2
	v_cmp_lt_u32_e64 s1, 0x7f, v3
	s_or_b32 s36, s1, s36
	s_andn2_b32 exec_lo, exec_lo, s36
	s_cbranch_execz .LBB0_42
.LBB0_40:                               ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_add_nc_u32_e32 v0, s3, v3
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s1, s6, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s1, s37, s1
	s_and_saveexec_b32 s38, s1
	s_cbranch_execz .LBB0_39
; %bb.41:                               ;   in Loop: Header=BB0_40 Depth=2
	v_add_nc_u32_e32 v64, s22, v3
	v_lshlrev_b64 v[0:1], 3, v[64:65]
	v_add_co_u32 v0, s1, s10, v0
	v_add_co_ci_u32_e64 v1, s1, s11, v1, s1
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_39
.LBB0_42:                               ;   in Loop: Header=BB0_3 Depth=1
	s_or_b32 exec_lo, exec_lo, s36
	s_or_b32 s1, s35, 4
	v_mov_b32_e32 v2, v79
	v_mov_b32_e32 v3, v70
	s_cmp_lt_u32 s1, s16
	s_mov_b32 s36, 0
	s_cselect_b32 s37, -1, 0
	s_branch .LBB0_44
.LBB0_43:                               ;   in Loop: Header=BB0_44 Depth=2
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s38
	v_add_nc_u32_e32 v3, s23, v3
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v2, v[0:1]
	v_add_nc_u32_e32 v2, s25, v2
	v_cmp_lt_u32_e64 s1, 0x7f, v3
	s_or_b32 s36, s1, s36
	s_andn2_b32 exec_lo, exec_lo, s36
	s_cbranch_execz .LBB0_46
.LBB0_44:                               ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_add_nc_u32_e32 v0, s3, v3
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s1, s6, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s1, s37, s1
	s_and_saveexec_b32 s38, s1
	s_cbranch_execz .LBB0_43
; %bb.45:                               ;   in Loop: Header=BB0_44 Depth=2
	v_add_nc_u32_e32 v64, s26, v3
	v_lshlrev_b64 v[0:1], 3, v[64:65]
	v_add_co_u32 v0, s1, s10, v0
	v_add_co_ci_u32_e64 v1, s1, s11, v1, s1
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_43
.LBB0_46:                               ;   in Loop: Header=BB0_3 Depth=1
	s_or_b32 exec_lo, exec_lo, s36
	s_or_b32 s1, s35, 5
	v_mov_b32_e32 v2, v80
	v_mov_b32_e32 v3, v70
	s_cmp_lt_u32 s1, s16
	s_mov_b32 s36, 0
	s_cselect_b32 s37, -1, 0
	s_branch .LBB0_48
.LBB0_47:                               ;   in Loop: Header=BB0_48 Depth=2
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s38
	v_add_nc_u32_e32 v3, s23, v3
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v2, v[0:1]
	v_add_nc_u32_e32 v2, s25, v2
	v_cmp_lt_u32_e64 s1, 0x7f, v3
	s_or_b32 s36, s1, s36
	s_andn2_b32 exec_lo, exec_lo, s36
	s_cbranch_execz .LBB0_50
.LBB0_48:                               ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_add_nc_u32_e32 v0, s3, v3
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s1, s6, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s1, s37, s1
	s_and_saveexec_b32 s38, s1
	s_cbranch_execz .LBB0_47
; %bb.49:                               ;   in Loop: Header=BB0_48 Depth=2
	v_add_nc_u32_e32 v64, s27, v3
	v_lshlrev_b64 v[0:1], 3, v[64:65]
	v_add_co_u32 v0, s1, s10, v0
	v_add_co_ci_u32_e64 v1, s1, s11, v1, s1
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_47
.LBB0_50:                               ;   in Loop: Header=BB0_3 Depth=1
	s_or_b32 exec_lo, exec_lo, s36
	s_or_b32 s1, s35, 6
	v_mov_b32_e32 v2, v81
	v_mov_b32_e32 v3, v70
	s_cmp_lt_u32 s1, s16
	s_mov_b32 s36, 0
	s_cselect_b32 s37, -1, 0
	s_branch .LBB0_52
.LBB0_51:                               ;   in Loop: Header=BB0_52 Depth=2
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s38
	v_add_nc_u32_e32 v3, s23, v3
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v2, v[0:1]
	v_add_nc_u32_e32 v2, s25, v2
	v_cmp_lt_u32_e64 s1, 0x7f, v3
	s_or_b32 s36, s1, s36
	s_andn2_b32 exec_lo, exec_lo, s36
	s_cbranch_execz .LBB0_54
.LBB0_52:                               ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_add_nc_u32_e32 v0, s3, v3
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s1, s6, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s1, s37, s1
	s_and_saveexec_b32 s38, s1
	s_cbranch_execz .LBB0_51
; %bb.53:                               ;   in Loop: Header=BB0_52 Depth=2
	v_add_nc_u32_e32 v64, s28, v3
	v_lshlrev_b64 v[0:1], 3, v[64:65]
	v_add_co_u32 v0, s1, s10, v0
	v_add_co_ci_u32_e64 v1, s1, s11, v1, s1
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_51
.LBB0_54:                               ;   in Loop: Header=BB0_3 Depth=1
	s_or_b32 exec_lo, exec_lo, s36
	s_or_b32 s1, s35, 7
	v_mov_b32_e32 v2, v82
	v_mov_b32_e32 v3, v70
	s_cmp_lt_u32 s1, s16
	s_mov_b32 s35, 0
	s_cselect_b32 s36, -1, 0
	s_branch .LBB0_56
.LBB0_55:                               ;   in Loop: Header=BB0_56 Depth=2
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s37
	v_add_nc_u32_e32 v3, s23, v3
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v2, v[0:1]
	v_add_nc_u32_e32 v2, s25, v2
	v_cmp_lt_u32_e64 s1, 0x7f, v3
	s_or_b32 s35, s1, s35
	s_andn2_b32 exec_lo, exec_lo, s35
	s_cbranch_execz .LBB0_2
.LBB0_56:                               ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_add_nc_u32_e32 v0, s3, v3
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s1, s6, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s1, s36, s1
	s_and_saveexec_b32 s37, s1
	s_cbranch_execz .LBB0_55
; %bb.57:                               ;   in Loop: Header=BB0_56 Depth=2
	v_add_nc_u32_e32 v64, s29, v3
	v_lshlrev_b64 v[0:1], 3, v[64:65]
	v_add_co_u32 v0, s1, s10, v0
	v_add_co_ci_u32_e64 v1, s1, s11, v1, s1
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_55
.LBB0_58:                               ; %Flow492
	v_add_nc_u32_e32 v4, s2, v66
	v_add_nc_u32_e32 v0, s3, v67
	v_mul_lo_u32 v5, s6, v4
	v_cmp_gt_u32_e64 s3, s14, v4
	v_cmp_gt_u32_e32 vcc_lo, s6, v0
	s_and_b32 s0, s3, vcc_lo
	s_and_saveexec_b32 s1, s0
	s_cbranch_execz .LBB0_60
; %bb.59:
	v_add_nc_u32_e32 v1, v5, v0
	v_mov_b32_e32 v2, 0
	v_lshlrev_b64 v[1:2], 3, v[1:2]
	v_add_co_u32 v1, s0, s12, v1
	v_add_co_ci_u32_e64 v2, s0, s13, v2, s0
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[1:2], v[62:63], off
.LBB0_60:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s1
	v_or_b32_e32 v1, 1, v0
	v_cmp_gt_u32_e64 s0, s6, v1
	s_and_b32 s1, s3, s0
	s_and_saveexec_b32 s2, s1
	s_cbranch_execz .LBB0_62
; %bb.61:
	v_add_nc_u32_e32 v2, v5, v1
	v_mov_b32_e32 v3, 0
	v_lshlrev_b64 v[2:3], 3, v[2:3]
	v_add_co_u32 v2, s1, s12, v2
	v_add_co_ci_u32_e64 v3, s1, s13, v3, s1
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[2:3], v[60:61], off
.LBB0_62:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s2
	v_or_b32_e32 v2, 2, v0
	v_cmp_gt_u32_e64 s1, s6, v2
	s_and_b32 s2, s3, s1
	s_and_saveexec_b32 s4, s2
	s_cbranch_execz .LBB0_64
; %bb.63:
	v_add_nc_u32_e32 v6, v5, v2
	v_mov_b32_e32 v7, 0
	v_lshlrev_b64 v[6:7], 3, v[6:7]
	v_add_co_u32 v6, s2, s12, v6
	v_add_co_ci_u32_e64 v7, s2, s13, v7, s2
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[6:7], v[58:59], off
.LBB0_64:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	v_or_b32_e32 v3, 3, v0
	v_cmp_gt_u32_e64 s2, s6, v3
	s_and_b32 s3, s3, s2
	s_and_saveexec_b32 s4, s3
	s_cbranch_execz .LBB0_66
; %bb.65:
	v_add_nc_u32_e32 v6, v5, v3
	v_mov_b32_e32 v7, 0
	v_lshlrev_b64 v[6:7], 3, v[6:7]
	v_add_co_u32 v6, s3, s12, v6
	v_add_co_ci_u32_e64 v7, s3, s13, v7, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[6:7], v[56:57], off
.LBB0_66:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	v_or_b32_e32 v6, 1, v4
	v_add_nc_u32_e32 v5, s6, v5
	v_cmp_gt_u32_e64 s3, s14, v6
	s_and_b32 s4, s3, vcc_lo
	s_and_saveexec_b32 s5, s4
	s_cbranch_execnz .LBB0_81
; %bb.67:
	s_or_b32 exec_lo, exec_lo, s5
	s_and_b32 s4, s3, s0
	s_and_saveexec_b32 s5, s4
	s_cbranch_execnz .LBB0_82
.LBB0_68:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s5
	s_and_b32 s4, s3, s1
	s_and_saveexec_b32 s5, s4
	s_cbranch_execnz .LBB0_83
.LBB0_69:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s5
	s_and_b32 s3, s3, s2
	s_and_saveexec_b32 s4, s3
	s_cbranch_execz .LBB0_71
.LBB0_70:
	v_add_nc_u32_e32 v6, v5, v3
	v_mov_b32_e32 v7, 0
	v_lshlrev_b64 v[6:7], 3, v[6:7]
	v_add_co_u32 v6, s3, s12, v6
	v_add_co_ci_u32_e64 v7, s3, s13, v7, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[6:7], v[48:49], off
.LBB0_71:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	v_or_b32_e32 v6, 2, v4
	v_add_nc_u32_e32 v5, s6, v5
	v_cmp_gt_u32_e64 s3, s14, v6
	s_and_b32 s4, s3, vcc_lo
	s_and_saveexec_b32 s5, s4
	s_cbranch_execnz .LBB0_84
; %bb.72:
	s_or_b32 exec_lo, exec_lo, s5
	s_and_b32 s4, s3, s0
	s_and_saveexec_b32 s5, s4
	s_cbranch_execnz .LBB0_85
.LBB0_73:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s5
	s_and_b32 s4, s3, s1
	s_and_saveexec_b32 s5, s4
	s_cbranch_execnz .LBB0_86
.LBB0_74:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s5
	s_and_b32 s3, s3, s2
	s_and_saveexec_b32 s4, s3
	s_cbranch_execz .LBB0_76
.LBB0_75:
	v_add_nc_u32_e32 v6, v5, v3
	v_mov_b32_e32 v7, 0
	v_lshlrev_b64 v[6:7], 3, v[6:7]
	v_add_co_u32 v6, s3, s12, v6
	v_add_co_ci_u32_e64 v7, s3, s13, v7, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[6:7], v[40:41], off
.LBB0_76:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	v_or_b32_e32 v4, 3, v4
	v_cmp_gt_u32_e64 s3, s14, v4
	v_add_nc_u32_e32 v4, s6, v5
	s_and_b32 s5, s3, vcc_lo
	s_and_saveexec_b32 s4, s5
	s_cbranch_execnz .LBB0_87
; %bb.77:
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s4, s3, s0
	s_and_saveexec_b32 s0, s4
	s_cbranch_execnz .LBB0_88
.LBB0_78:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s0
	s_and_b32 s1, s3, s1
	s_and_saveexec_b32 s0, s1
	s_cbranch_execnz .LBB0_89
.LBB0_79:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s0
	s_and_b32 s0, s3, s2
	s_and_saveexec_b32 s1, s0
	s_cbranch_execnz .LBB0_90
.LBB0_80:
	s_endpgm
.LBB0_81:
	v_add_nc_u32_e32 v6, v5, v0
	v_mov_b32_e32 v7, 0
	v_lshlrev_b64 v[6:7], 3, v[6:7]
	v_add_co_u32 v6, s4, s12, v6
	v_add_co_ci_u32_e64 v7, s4, s13, v7, s4
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[6:7], v[54:55], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s5
	s_and_b32 s4, s3, s0
	s_and_saveexec_b32 s5, s4
	s_cbranch_execz .LBB0_68
.LBB0_82:
	v_add_nc_u32_e32 v6, v5, v1
	v_mov_b32_e32 v7, 0
	v_lshlrev_b64 v[6:7], 3, v[6:7]
	v_add_co_u32 v6, s4, s12, v6
	v_add_co_ci_u32_e64 v7, s4, s13, v7, s4
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[6:7], v[52:53], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s5
	s_and_b32 s4, s3, s1
	s_and_saveexec_b32 s5, s4
	s_cbranch_execz .LBB0_69
.LBB0_83:
	v_add_nc_u32_e32 v6, v5, v2
	v_mov_b32_e32 v7, 0
	v_lshlrev_b64 v[6:7], 3, v[6:7]
	v_add_co_u32 v6, s4, s12, v6
	v_add_co_ci_u32_e64 v7, s4, s13, v7, s4
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[6:7], v[50:51], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s5
	s_and_b32 s3, s3, s2
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_70
	s_branch .LBB0_71
.LBB0_84:
	v_add_nc_u32_e32 v6, v5, v0
	v_mov_b32_e32 v7, 0
	v_lshlrev_b64 v[6:7], 3, v[6:7]
	v_add_co_u32 v6, s4, s12, v6
	v_add_co_ci_u32_e64 v7, s4, s13, v7, s4
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[6:7], v[46:47], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s5
	s_and_b32 s4, s3, s0
	s_and_saveexec_b32 s5, s4
	s_cbranch_execz .LBB0_73
.LBB0_85:
	v_add_nc_u32_e32 v6, v5, v1
	v_mov_b32_e32 v7, 0
	v_lshlrev_b64 v[6:7], 3, v[6:7]
	v_add_co_u32 v6, s4, s12, v6
	v_add_co_ci_u32_e64 v7, s4, s13, v7, s4
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[6:7], v[44:45], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s5
	s_and_b32 s4, s3, s1
	s_and_saveexec_b32 s5, s4
	s_cbranch_execz .LBB0_74
.LBB0_86:
	v_add_nc_u32_e32 v6, v5, v2
	v_mov_b32_e32 v7, 0
	v_lshlrev_b64 v[6:7], 3, v[6:7]
	v_add_co_u32 v6, s4, s12, v6
	v_add_co_ci_u32_e64 v7, s4, s13, v7, s4
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[6:7], v[42:43], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s5
	s_and_b32 s3, s3, s2
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_75
	s_branch .LBB0_76
.LBB0_87:
	v_add_nc_u32_e32 v5, v4, v0
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, vcc_lo, s12, v5
	v_add_co_ci_u32_e32 v6, vcc_lo, s13, v6, vcc_lo
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[38:39], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s4, s3, s0
	s_and_saveexec_b32 s0, s4
	s_cbranch_execz .LBB0_78
.LBB0_88:
	v_add_nc_u32_e32 v0, v4, v1
	v_mov_b32_e32 v1, 0
	v_lshlrev_b64 v[0:1], 3, v[0:1]
	v_add_co_u32 v0, vcc_lo, s12, v0
	v_add_co_ci_u32_e32 v1, vcc_lo, s13, v1, vcc_lo
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[0:1], v[36:37], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s0
	s_and_b32 s1, s3, s1
	s_and_saveexec_b32 s0, s1
	s_cbranch_execz .LBB0_79
.LBB0_89:
	v_add_nc_u32_e32 v0, v4, v2
	v_mov_b32_e32 v1, 0
	v_lshlrev_b64 v[0:1], 3, v[0:1]
	v_add_co_u32 v0, vcc_lo, s12, v0
	v_add_co_ci_u32_e32 v1, vcc_lo, s13, v1, vcc_lo
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[0:1], v[34:35], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s0
	s_and_b32 s0, s3, s2
	s_and_saveexec_b32 s1, s0
	s_cbranch_execz .LBB0_80
.LBB0_90:
	v_add_nc_u32_e32 v0, v4, v3
	v_mov_b32_e32 v1, 0
	v_lshlrev_b64 v[0:1], 3, v[0:1]
	v_add_co_u32 v0, vcc_lo, s12, v0
	v_add_co_ci_u32_e32 v1, vcc_lo, s13, v1, vcc_lo
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[0:1], v[32:33], off
	s_endpgm
	.section	.rodata,"a",@progbits
	.p2align	6, 0x0
	.amdhsa_kernel mul
		.amdhsa_group_segment_fixed_size 8704
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
		.amdhsa_next_free_vgpr 95
		.amdhsa_next_free_sgpr 43
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
; codeLenInByte = 5744
; NumSgprs: 45
; NumVgprs: 95
; ScratchSize: 0
; MemoryBound: 0
; FloatMode: 240
; IeeeMode: 1
; LDSByteSize: 8704 bytes/workgroup (compile time only)
; SGPRBlocks: 5
; VGPRBlocks: 11
; NumSGPRsForWavesPerEU: 45
; NumVGPRsForWavesPerEU: 95
; Occupancy: 10
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
    .group_segment_fixed_size: 8704
    .kernarg_segment_align: 8
    .kernarg_segment_size: 296
    .language:       OpenCL C
    .language_version:
      - 1
      - 2
    .max_flat_workgroup_size: 256
    .name:           mul
    .private_segment_fixed_size: 0
    .sgpr_count:     45
    .sgpr_spill_count: 0
    .symbol:         mul.kd
    .uniform_work_group_size: 1
    .uses_dynamic_stack: false
    .vgpr_count:     95
    .vgpr_spill_count: 0
    .wavefront_size: 32
    .workgroup_processor_mode: 1
amdhsa.target:   'amdgcn-amd-amdhsa--gfx1010:xnack-'
amdhsa.version:
  - 1
  - 2
...

	.end_amdgpu_metadata
