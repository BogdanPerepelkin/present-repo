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
	s_load_dword s16, s[4:5], 0x20
	s_load_dwordx8 s[8:15], s[4:5], 0x0
	s_lshl_b32 s2, s7, 4
	v_mov_b32_e32 v85, 0
	v_mov_b32_e32 v84, 0
	v_mov_b32_e32 v83, 0
	v_mov_b32_e32 v82, 0
	v_mov_b32_e32 v81, 0
	v_mov_b32_e32 v80, 0
	v_mov_b32_e32 v79, 0
	v_mov_b32_e32 v78, 0
	v_mov_b32_e32 v77, 0
	v_mov_b32_e32 v76, 0
	v_mov_b32_e32 v75, 0
	v_mov_b32_e32 v74, 0
	v_mov_b32_e32 v73, 0
	v_mov_b32_e32 v72, 0
	v_mov_b32_e32 v71, 0
	v_mov_b32_e32 v70, 0
	v_lshlrev_b32_e32 v88, 3, v1
	s_waitcnt lgkmcnt(0)
	s_mul_hi_u32 s0, s16, 0xaaaaaaab
	s_sub_i32 s1, 0, s16
	s_lshr_b32 s0, s0, 2
	s_lshr_b32 s7, s15, 1
	s_mul_i32 s3, s0, -6
	v_mul_u32_u24_e32 v89, 3, v0
	s_cmp_lg_u32 s3, s1
	v_mov_b32_e32 v69, 0
	s_cselect_b32 s1, -1, 0
	v_mov_b32_e32 v68, 0
	s_cmp_lg_u32 s1, 0
	v_mov_b32_e32 v67, 0
	v_mov_b32_e32 v66, 0
	v_mov_b32_e32 v65, 0
	v_mov_b32_e32 v64, 0
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
	v_mov_b32_e32 v50, 0
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
	s_addc_u32 s15, s0, 0
	s_mul_i32 s3, s6, 48
	s_cmp_eq_u32 s15, 0
	s_cbranch_scc1 .LBB0_32
; %bb.1:
	s_load_dword s0, s[4:5], 0x34
	v_lshlrev_b32_e32 v90, 3, v89
	v_mad_u32_u24 v91, v88, 24, 0x900
	v_mov_b32_e32 v38, 0
	v_mov_b32_e32 v87, 0
	v_mov_b32_e32 v92, 3
	v_mov_b32_e32 v39, 0
	v_mov_b32_e32 v40, 0
	v_mov_b32_e32 v41, 0
	v_mov_b32_e32 v42, 0
	v_mov_b32_e32 v43, 0
	v_mov_b32_e32 v44, 0
	v_mov_b32_e32 v45, 0
	v_mov_b32_e32 v46, 0
	v_mov_b32_e32 v47, 0
	v_mov_b32_e32 v48, 0
	v_mov_b32_e32 v49, 0
	v_mov_b32_e32 v50, 0
	v_mov_b32_e32 v51, 0
	v_mov_b32_e32 v52, 0
	s_waitcnt lgkmcnt(0)
	s_and_b32 s19, s0, 0xffff
	s_lshr_b32 s0, s0, 16
	v_mad_u32_u24 v93, v1, s19, v0
	v_mov_b32_e32 v53, 0
	v_mov_b32_e32 v54, 0
	v_mov_b32_e32 v55, 0
	v_mov_b32_e32 v56, 0
	v_lshlrev_b32_e32 v94, 3, v93
	v_mov_b32_e32 v57, 0
	v_mov_b32_e32 v58, 0
	v_mov_b32_e32 v59, 0
	v_mov_b32_e32 v60, 0
	v_add_nc_u32_e32 v95, 0x180, v94
	v_add_nc_u32_e32 v96, 0x300, v94
	v_add_nc_u32_e32 v97, 0x480, v94
	v_add_nc_u32_e32 v98, 0x600, v94
	v_add_nc_u32_e32 v99, 0x780, v94
	v_mov_b32_e32 v61, 0
	v_mov_b32_e32 v62, 0
	v_mov_b32_e32 v63, 0
	v_mov_b32_e32 v64, 0
	v_mov_b32_e32 v65, 0
	v_mov_b32_e32 v66, 0
	v_mov_b32_e32 v67, 0
	v_mov_b32_e32 v68, 0
	v_mov_b32_e32 v69, 0
	v_mov_b32_e32 v70, 0
	v_mov_b32_e32 v71, 0
	v_mov_b32_e32 v72, 0
	v_mov_b32_e32 v73, 0
	v_mov_b32_e32 v74, 0
	v_mov_b32_e32 v75, 0
	v_mov_b32_e32 v76, 0
	v_mov_b32_e32 v77, 0
	v_mov_b32_e32 v78, 0
	v_mov_b32_e32 v79, 0
	v_mov_b32_e32 v80, 0
	v_mov_b32_e32 v81, 0
	v_mov_b32_e32 v82, 0
	v_mov_b32_e32 v83, 0
	v_mov_b32_e32 v84, 0
	v_mov_b32_e32 v85, 0
	s_lshl_b32 s1, s7, 1
	s_mul_i32 s18, s7, 3
	s_lshl_b32 s20, s7, 2
	s_mul_i32 s22, s7, 5
	s_mul_i32 s19, s0, s19
	s_lshr_b32 s4, s16, 1
	s_mul_i32 s5, s7, 6
	s_add_i32 s6, s7, s3
	s_add_i32 s17, s3, s1
	s_add_i32 s18, s3, s18
	s_add_i32 s20, s3, s20
	s_lshl_b32 s21, s19, 3
	s_add_i32 s22, s3, s22
	s_mov_b32 s23, 0
	s_mov_b32 s24, s3
	v_cmp_gt_u32_e32 vcc_lo, 48, v93
	s_branch .LBB0_3
.LBB0_2:                                ; %Flow521
                                        ;   in Loop: Header=BB0_3 Depth=1
	s_or_b32 exec_lo, exec_lo, s25
	s_waitcnt lgkmcnt(0)
	; wave barrier
	s_waitcnt lgkmcnt(0)
	ds_read2_b64 v[16:19], v90 offset1:1
	ds_read2_b64 v[20:23], v90 offset0:2 offset1:48
	ds_read2_b64 v[28:31], v91 offset1:1
	ds_read2_b64 v[4:7], v91 offset0:2 offset1:3
	ds_read2_b64 v[0:3], v91 offset0:4 offset1:5
	ds_read2_b64 v[34:37], v91 offset0:6 offset1:7
	ds_read2_b64 v[24:27], v90 offset0:49 offset1:50
	ds_read2_b64 v[12:15], v91 offset0:14 offset1:15
	s_add_i32 s23, s23, 1
	s_add_i32 s24, s24, s5
	s_add_i32 s6, s6, s5
	s_add_i32 s17, s17, s5
	s_add_i32 s18, s18, s5
	s_add_i32 s20, s20, s5
	s_add_i32 s22, s22, s5
	s_cmp_eq_u32 s23, s15
	ds_read2_b64 v[100:103], v91 offset0:12 offset1:13
	s_waitcnt lgkmcnt(6)
	v_fmac_f32_e32 v85, v28, v17
	s_waitcnt lgkmcnt(5)
	v_fmac_f32_e32 v79, v6, v17
	v_fmac_f32_e32 v78, v6, v16
	v_fmac_f32_e32 v77, v6, v19
	v_fmac_f32_e32 v76, v6, v18
	v_fmac_f32_e32 v75, v6, v21
	v_fmac_f32_e32 v74, v6, v20
	v_fmac_f32_e32 v78, v7, v22
	v_fmac_f32_e32 v79, v7, v23
	s_waitcnt lgkmcnt(2)
	v_fmac_f32_e32 v76, v7, v24
	v_fmac_f32_e32 v77, v7, v25
	v_fmac_f32_e32 v74, v7, v26
	v_fmac_f32_e32 v75, v7, v27
	ds_read2_b64 v[6:9], v91 offset0:8 offset1:9
	v_fmac_f32_e32 v73, v34, v17
	v_fmac_f32_e32 v72, v34, v16
	v_fmac_f32_e32 v71, v34, v19
	v_fmac_f32_e32 v70, v34, v18
	v_fmac_f32_e32 v69, v34, v21
	v_fmac_f32_e32 v68, v34, v20
	v_fmac_f32_e32 v72, v35, v22
	v_fmac_f32_e32 v73, v35, v23
	v_fmac_f32_e32 v70, v35, v24
	v_fmac_f32_e32 v71, v35, v25
	v_fmac_f32_e32 v68, v35, v26
	v_fmac_f32_e32 v69, v35, v27
	ds_read2_b64 v[32:35], v91 offset0:18 offset1:19
	v_fmac_f32_e32 v84, v28, v16
	v_fmac_f32_e32 v83, v28, v19
	v_fmac_f32_e32 v82, v28, v18
	v_fmac_f32_e32 v81, v28, v21
	v_fmac_f32_e32 v80, v28, v20
	s_waitcnt lgkmcnt(2)
	v_fmac_f32_e32 v57, v100, v21
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v67, v8, v17
	v_fmac_f32_e32 v66, v8, v16
	v_fmac_f32_e32 v65, v8, v19
	v_fmac_f32_e32 v64, v8, v18
	v_fmac_f32_e32 v63, v8, v21
	v_fmac_f32_e32 v62, v8, v20
	v_fmac_f32_e32 v66, v9, v22
	v_fmac_f32_e32 v67, v9, v23
	v_fmac_f32_e32 v64, v9, v24
	v_fmac_f32_e32 v65, v9, v25
	v_fmac_f32_e32 v62, v9, v26
	v_fmac_f32_e32 v63, v9, v27
	ds_read2_b64 v[8:11], v91 offset0:20 offset1:21
	v_fmac_f32_e32 v56, v100, v20
	v_fmac_f32_e32 v51, v14, v21
	v_fmac_f32_e32 v50, v14, v20
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v45, v32, v21
	v_fmac_f32_e32 v44, v32, v20
	v_fmac_f32_e32 v84, v29, v22
	v_fmac_f32_e32 v85, v29, v23
	v_fmac_f32_e32 v82, v29, v24
	v_fmac_f32_e32 v83, v29, v25
	v_fmac_f32_e32 v80, v29, v26
	v_fmac_f32_e32 v81, v29, v27
	v_fmac_f32_e32 v61, v100, v17
	v_fmac_f32_e32 v60, v100, v16
	v_fmac_f32_e32 v59, v100, v19
	v_fmac_f32_e32 v58, v100, v18
	v_fmac_f32_e32 v56, v101, v26
	v_fmac_f32_e32 v57, v101, v27
	v_fmac_f32_e32 v55, v14, v17
	v_fmac_f32_e32 v54, v14, v16
	s_waitcnt lgkmcnt(0)
	v_fmac_f32_e32 v39, v10, v21
	v_fmac_f32_e32 v38, v10, v20
	v_fmac_f32_e32 v53, v14, v19
	v_fmac_f32_e32 v52, v14, v18
	v_fmac_f32_e32 v50, v15, v26
	v_fmac_f32_e32 v51, v15, v27
	v_fmac_f32_e32 v49, v32, v17
	v_fmac_f32_e32 v48, v32, v16
	v_fmac_f32_e32 v47, v32, v19
	v_fmac_f32_e32 v46, v32, v18
	v_fmac_f32_e32 v44, v33, v26
	v_fmac_f32_e32 v45, v33, v27
	v_fmac_f32_e32 v43, v10, v17
	v_fmac_f32_e32 v42, v10, v16
	v_fmac_f32_e32 v41, v10, v19
	v_fmac_f32_e32 v40, v10, v18
	v_fmac_f32_e32 v38, v11, v26
	v_fmac_f32_e32 v39, v11, v27
	ds_read2_b64 v[26:29], v90 offset0:96 offset1:97
	ds_read2_b64 v[18:21], v90 offset0:98 offset1:144
	v_fmac_f32_e32 v60, v101, v22
	v_fmac_f32_e32 v61, v101, v23
	v_fmac_f32_e32 v58, v101, v24
	v_fmac_f32_e32 v59, v101, v25
	v_fmac_f32_e32 v54, v15, v22
	v_fmac_f32_e32 v55, v15, v23
	v_fmac_f32_e32 v52, v15, v24
	v_fmac_f32_e32 v53, v15, v25
	v_fmac_f32_e32 v48, v33, v22
	v_fmac_f32_e32 v49, v33, v23
	v_fmac_f32_e32 v46, v33, v24
	v_fmac_f32_e32 v47, v33, v25
	v_fmac_f32_e32 v42, v11, v22
	v_fmac_f32_e32 v43, v11, v23
	v_fmac_f32_e32 v40, v11, v24
	v_fmac_f32_e32 v41, v11, v25
	ds_read2_b64 v[22:25], v90 offset0:145 offset1:146
	ds_read2_b64 v[14:17], v91 offset0:10 offset1:11
	s_waitcnt lgkmcnt(3)
	v_fmac_f32_e32 v85, v30, v27
	v_fmac_f32_e32 v84, v30, v26
	v_fmac_f32_e32 v83, v30, v29
	v_fmac_f32_e32 v82, v30, v28
	s_waitcnt lgkmcnt(2)
	v_fmac_f32_e32 v81, v30, v19
	v_fmac_f32_e32 v80, v30, v18
	v_fmac_f32_e32 v73, v36, v27
	v_fmac_f32_e32 v72, v36, v26
	v_fmac_f32_e32 v71, v36, v29
	v_fmac_f32_e32 v70, v36, v28
	v_fmac_f32_e32 v69, v36, v19
	v_fmac_f32_e32 v68, v36, v18
	v_fmac_f32_e32 v49, v34, v27
	v_fmac_f32_e32 v48, v34, v26
	v_fmac_f32_e32 v47, v34, v29
	v_fmac_f32_e32 v46, v34, v28
	v_fmac_f32_e32 v45, v34, v19
	v_fmac_f32_e32 v44, v34, v18
	v_fmac_f32_e32 v84, v31, v20
	v_fmac_f32_e32 v85, v31, v21
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v82, v31, v22
	v_fmac_f32_e32 v83, v31, v23
	v_fmac_f32_e32 v80, v31, v24
	v_fmac_f32_e32 v81, v31, v25
	ds_read2_b64 v[30:33], v91 offset0:16 offset1:17
	v_fmac_f32_e32 v72, v37, v20
	v_fmac_f32_e32 v73, v37, v21
	v_fmac_f32_e32 v70, v37, v22
	v_fmac_f32_e32 v71, v37, v23
	v_fmac_f32_e32 v68, v37, v24
	v_fmac_f32_e32 v69, v37, v25
	v_fmac_f32_e32 v48, v35, v20
	v_fmac_f32_e32 v49, v35, v21
	v_fmac_f32_e32 v46, v35, v22
	v_fmac_f32_e32 v47, v35, v23
	v_fmac_f32_e32 v44, v35, v24
	v_fmac_f32_e32 v45, v35, v25
	ds_read2_b64 v[34:37], v91 offset0:22 offset1:23
	v_fmac_f32_e32 v79, v0, v27
	v_fmac_f32_e32 v78, v0, v26
	v_fmac_f32_e32 v77, v0, v29
	v_fmac_f32_e32 v76, v0, v28
	v_fmac_f32_e32 v75, v0, v19
	v_fmac_f32_e32 v74, v0, v18
	s_waitcnt lgkmcnt(2)
	v_fmac_f32_e32 v67, v14, v27
	v_fmac_f32_e32 v66, v14, v26
	v_fmac_f32_e32 v65, v14, v29
	v_fmac_f32_e32 v64, v14, v28
	v_fmac_f32_e32 v63, v14, v19
	v_fmac_f32_e32 v62, v14, v18
	v_fmac_f32_e32 v61, v102, v27
	v_fmac_f32_e32 v60, v102, v26
	v_fmac_f32_e32 v59, v102, v29
	v_fmac_f32_e32 v58, v102, v28
	v_fmac_f32_e32 v57, v102, v19
	v_fmac_f32_e32 v56, v102, v18
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v55, v30, v27
	v_fmac_f32_e32 v54, v30, v26
	v_fmac_f32_e32 v53, v30, v29
	v_fmac_f32_e32 v52, v30, v28
	v_fmac_f32_e32 v51, v30, v19
	v_fmac_f32_e32 v50, v30, v18
	s_waitcnt lgkmcnt(0)
	v_fmac_f32_e32 v43, v34, v27
	v_fmac_f32_e32 v42, v34, v26
	v_fmac_f32_e32 v41, v34, v29
	v_fmac_f32_e32 v40, v34, v28
	v_fmac_f32_e32 v39, v34, v19
	v_fmac_f32_e32 v38, v34, v18
	v_fmac_f32_e32 v78, v1, v20
	v_fmac_f32_e32 v79, v1, v21
	v_fmac_f32_e32 v76, v1, v22
	v_fmac_f32_e32 v77, v1, v23
	v_fmac_f32_e32 v74, v1, v24
	v_fmac_f32_e32 v75, v1, v25
	v_fmac_f32_e32 v66, v15, v20
	v_fmac_f32_e32 v67, v15, v21
	v_fmac_f32_e32 v64, v15, v22
	v_fmac_f32_e32 v65, v15, v23
	v_fmac_f32_e32 v62, v15, v24
	v_fmac_f32_e32 v63, v15, v25
	v_fmac_f32_e32 v60, v103, v20
	v_fmac_f32_e32 v61, v103, v21
	v_fmac_f32_e32 v58, v103, v22
	v_fmac_f32_e32 v59, v103, v23
	v_fmac_f32_e32 v56, v103, v24
	v_fmac_f32_e32 v57, v103, v25
	v_fmac_f32_e32 v54, v31, v20
	v_fmac_f32_e32 v55, v31, v21
	v_fmac_f32_e32 v52, v31, v22
	v_fmac_f32_e32 v53, v31, v23
	v_fmac_f32_e32 v50, v31, v24
	v_fmac_f32_e32 v51, v31, v25
	v_fmac_f32_e32 v42, v35, v20
	v_fmac_f32_e32 v43, v35, v21
	v_fmac_f32_e32 v40, v35, v22
	v_fmac_f32_e32 v41, v35, v23
	v_fmac_f32_e32 v38, v35, v24
	v_fmac_f32_e32 v39, v35, v25
	ds_read2_b64 v[18:21], v90 offset0:192 offset1:193
	ds_read2_b64 v[26:29], v90 offset0:241 offset1:242
	ds_read2_b64 v[22:25], v90 offset0:194 offset1:240
	s_waitcnt lgkmcnt(0)
	; wave barrier
	s_waitcnt lgkmcnt(0)
	v_fmac_f32_e32 v85, v4, v19
	v_fmac_f32_e32 v84, v4, v18
	v_fmac_f32_e32 v83, v4, v21
	v_fmac_f32_e32 v82, v4, v20
	v_fmac_f32_e32 v81, v4, v23
	v_fmac_f32_e32 v80, v4, v22
	v_fmac_f32_e32 v79, v2, v19
	v_fmac_f32_e32 v78, v2, v18
	v_fmac_f32_e32 v77, v2, v21
	v_fmac_f32_e32 v76, v2, v20
	v_fmac_f32_e32 v75, v2, v23
	v_fmac_f32_e32 v74, v2, v22
	v_fmac_f32_e32 v73, v6, v19
	v_fmac_f32_e32 v72, v6, v18
	v_fmac_f32_e32 v71, v6, v21
	v_fmac_f32_e32 v70, v6, v20
	v_fmac_f32_e32 v69, v6, v23
	v_fmac_f32_e32 v68, v6, v22
	v_fmac_f32_e32 v67, v16, v19
	v_fmac_f32_e32 v66, v16, v18
	v_fmac_f32_e32 v65, v16, v21
	v_fmac_f32_e32 v64, v16, v20
	v_fmac_f32_e32 v63, v16, v23
	v_fmac_f32_e32 v62, v16, v22
	v_fmac_f32_e32 v61, v12, v19
	v_fmac_f32_e32 v60, v12, v18
	v_fmac_f32_e32 v59, v12, v21
	v_fmac_f32_e32 v58, v12, v20
	v_fmac_f32_e32 v57, v12, v23
	v_fmac_f32_e32 v56, v12, v22
	v_fmac_f32_e32 v55, v32, v19
	v_fmac_f32_e32 v54, v32, v18
	v_fmac_f32_e32 v53, v32, v21
	v_fmac_f32_e32 v52, v32, v20
	v_fmac_f32_e32 v51, v32, v23
	v_fmac_f32_e32 v50, v32, v22
	v_fmac_f32_e32 v49, v8, v19
	v_fmac_f32_e32 v48, v8, v18
	v_fmac_f32_e32 v47, v8, v21
	v_fmac_f32_e32 v46, v8, v20
	v_fmac_f32_e32 v45, v8, v23
	v_fmac_f32_e32 v44, v8, v22
	v_fmac_f32_e32 v43, v36, v19
	v_fmac_f32_e32 v42, v36, v18
	v_fmac_f32_e32 v41, v36, v21
	v_fmac_f32_e32 v40, v36, v20
	v_fmac_f32_e32 v39, v36, v23
	v_fmac_f32_e32 v38, v36, v22
	v_fmac_f32_e32 v84, v5, v24
	v_fmac_f32_e32 v85, v5, v25
	v_fmac_f32_e32 v82, v5, v26
	v_fmac_f32_e32 v83, v5, v27
	v_fmac_f32_e32 v80, v5, v28
	v_fmac_f32_e32 v81, v5, v29
	v_fmac_f32_e32 v78, v3, v24
	v_fmac_f32_e32 v79, v3, v25
	v_fmac_f32_e32 v76, v3, v26
	v_fmac_f32_e32 v77, v3, v27
	v_fmac_f32_e32 v74, v3, v28
	v_fmac_f32_e32 v75, v3, v29
	v_fmac_f32_e32 v72, v7, v24
	v_fmac_f32_e32 v73, v7, v25
	v_fmac_f32_e32 v70, v7, v26
	v_fmac_f32_e32 v71, v7, v27
	v_fmac_f32_e32 v68, v7, v28
	v_fmac_f32_e32 v69, v7, v29
	v_fmac_f32_e32 v66, v17, v24
	v_fmac_f32_e32 v67, v17, v25
	v_fmac_f32_e32 v64, v17, v26
	v_fmac_f32_e32 v65, v17, v27
	v_fmac_f32_e32 v62, v17, v28
	v_fmac_f32_e32 v63, v17, v29
	v_fmac_f32_e32 v60, v13, v24
	v_fmac_f32_e32 v61, v13, v25
	v_fmac_f32_e32 v58, v13, v26
	v_fmac_f32_e32 v59, v13, v27
	v_fmac_f32_e32 v56, v13, v28
	v_fmac_f32_e32 v57, v13, v29
	v_fmac_f32_e32 v54, v33, v24
	v_fmac_f32_e32 v55, v33, v25
	v_fmac_f32_e32 v52, v33, v26
	v_fmac_f32_e32 v53, v33, v27
	v_fmac_f32_e32 v50, v33, v28
	v_fmac_f32_e32 v51, v33, v29
	v_fmac_f32_e32 v48, v9, v24
	v_fmac_f32_e32 v49, v9, v25
	v_fmac_f32_e32 v46, v9, v26
	v_fmac_f32_e32 v47, v9, v27
	v_fmac_f32_e32 v44, v9, v28
	v_fmac_f32_e32 v45, v9, v29
	v_fmac_f32_e32 v42, v37, v24
	v_fmac_f32_e32 v43, v37, v25
	v_fmac_f32_e32 v40, v37, v26
	v_fmac_f32_e32 v41, v37, v27
	v_fmac_f32_e32 v38, v37, v28
	v_fmac_f32_e32 v39, v37, v29
	s_cbranch_scc1 .LBB0_32
.LBB0_3:                                ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB0_6 Depth 2
                                        ;     Child Loop BB0_10 Depth 2
                                        ;     Child Loop BB0_14 Depth 2
                                        ;     Child Loop BB0_18 Depth 2
                                        ;     Child Loop BB0_22 Depth 2
                                        ;     Child Loop BB0_26 Depth 2
                                        ;     Child Loop BB0_30 Depth 2
	s_and_saveexec_b32 s25, vcc_lo
	s_cbranch_execz .LBB0_2
; %bb.4:                                ; %.preheader.preheader
                                        ;   in Loop: Header=BB0_3 Depth=1
	v_mov_b32_e32 v3, v93
	s_mul_i32 s27, s23, 3
	s_mov_b32 s26, 0
	s_branch .LBB0_6
.LBB0_5:                                ;   in Loop: Header=BB0_6 Depth=2
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s1
	v_add_nc_u32_e32 v3, s19, v3
	v_lshlrev_b32_sdwa v2, v92, v5 dst_sel:DWORD dst_unused:UNUSED_PAD src0_sel:DWORD src1_sel:BYTE_0
	v_cmp_lt_u32_e64 s0, 47, v3
	v_mad_u32_u24 v2, v4, 24, v2
	s_or_b32 s26, s0, s26
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v2, v[0:1] offset:2304
	s_waitcnt_depctr 0xffe3
	s_andn2_b32 exec_lo, exec_lo, s26
	s_cbranch_execz .LBB0_8
.LBB0_6:                                ; %.preheader
                                        ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_mul_lo_u16 v0, 0xab, v3
	v_mov_b32_e32 v1, 0
	v_lshrrev_b16 v0, 9, v0
	v_and_b32_e32 v4, 0xffff, v0
	v_mad_u16 v5, v0, -3, v3
	v_mov_b32_e32 v0, 0
	v_add_nc_u32_e32 v6, s2, v4
	v_add_nc_u32_sdwa v2, s27, v5 dst_sel:DWORD dst_unused:UNUSED_PAD src0_sel:DWORD src1_sel:BYTE_0
	v_cmp_gt_u32_e64 s0, s14, v6
	v_cmp_gt_u32_e64 s1, s4, v2
	s_and_b32 s0, s0, s1
	s_and_saveexec_b32 s1, s0
	s_cbranch_execz .LBB0_5
; %bb.7:                                ;   in Loop: Header=BB0_6 Depth=2
	v_mad_u64_u32 v[0:1], s0, v6, s4, v[2:3]
	v_mov_b32_e32 v1, v87
	v_lshlrev_b64 v[0:1], 3, v[0:1]
	v_add_co_u32 v0, s0, s8, v0
	v_add_co_ci_u32_e64 v1, s0, s9, v1, s0
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_5
.LBB0_8:                                ;   in Loop: Header=BB0_3 Depth=1
	s_or_b32 exec_lo, exec_lo, s26
	s_mul_i32 s1, s23, 6
	v_mov_b32_e32 v2, v94
	v_mov_b32_e32 v3, v93
	s_cmp_lt_u32 s1, s16
	s_mov_b32 s26, 0
	s_cselect_b32 s27, -1, 0
	s_branch .LBB0_10
.LBB0_9:                                ;   in Loop: Header=BB0_10 Depth=2
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s28
	v_add_nc_u32_e32 v3, s19, v3
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v2, v[0:1]
	v_add_nc_u32_e32 v2, s21, v2
	v_cmp_lt_u32_e64 s0, 47, v3
	s_or_b32 s26, s0, s26
	s_andn2_b32 exec_lo, exec_lo, s26
	s_cbranch_execz .LBB0_12
.LBB0_10:                               ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_add_nc_u32_e32 v0, s3, v3
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s0, s7, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s0, s27, s0
	s_and_saveexec_b32 s28, s0
	s_cbranch_execz .LBB0_9
; %bb.11:                               ;   in Loop: Header=BB0_10 Depth=2
	v_add_nc_u32_e32 v86, s24, v3
	v_lshlrev_b64 v[0:1], 3, v[86:87]
	v_add_co_u32 v0, s0, s10, v0
	v_add_co_ci_u32_e64 v1, s0, s11, v1, s0
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_9
.LBB0_12:                               ;   in Loop: Header=BB0_3 Depth=1
	s_or_b32 exec_lo, exec_lo, s26
	s_or_b32 s0, s1, 1
	v_mov_b32_e32 v2, v95
	v_mov_b32_e32 v3, v93
	s_cmp_lt_u32 s0, s16
	s_mov_b32 s26, 0
	s_cselect_b32 s27, -1, 0
	s_branch .LBB0_14
.LBB0_13:                               ;   in Loop: Header=BB0_14 Depth=2
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s28
	v_add_nc_u32_e32 v3, s19, v3
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v2, v[0:1]
	v_add_nc_u32_e32 v2, s21, v2
	v_cmp_lt_u32_e64 s0, 47, v3
	s_or_b32 s26, s0, s26
	s_andn2_b32 exec_lo, exec_lo, s26
	s_cbranch_execz .LBB0_16
.LBB0_14:                               ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_add_nc_u32_e32 v0, s3, v3
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s0, s7, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s0, s27, s0
	s_and_saveexec_b32 s28, s0
	s_cbranch_execz .LBB0_13
; %bb.15:                               ;   in Loop: Header=BB0_14 Depth=2
	v_add_nc_u32_e32 v86, s6, v3
	v_lshlrev_b64 v[0:1], 3, v[86:87]
	v_add_co_u32 v0, s0, s10, v0
	v_add_co_ci_u32_e64 v1, s0, s11, v1, s0
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_13
.LBB0_16:                               ;   in Loop: Header=BB0_3 Depth=1
	s_or_b32 exec_lo, exec_lo, s26
	s_add_i32 s0, s1, 2
	v_mov_b32_e32 v2, v96
	v_mov_b32_e32 v3, v93
	s_cmp_lt_u32 s0, s16
	s_mov_b32 s26, 0
	s_cselect_b32 s27, -1, 0
	s_branch .LBB0_18
.LBB0_17:                               ;   in Loop: Header=BB0_18 Depth=2
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s28
	v_add_nc_u32_e32 v3, s19, v3
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v2, v[0:1]
	v_add_nc_u32_e32 v2, s21, v2
	v_cmp_lt_u32_e64 s0, 47, v3
	s_or_b32 s26, s0, s26
	s_andn2_b32 exec_lo, exec_lo, s26
	s_cbranch_execz .LBB0_20
.LBB0_18:                               ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_add_nc_u32_e32 v0, s3, v3
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s0, s7, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s0, s27, s0
	s_and_saveexec_b32 s28, s0
	s_cbranch_execz .LBB0_17
; %bb.19:                               ;   in Loop: Header=BB0_18 Depth=2
	v_add_nc_u32_e32 v86, s17, v3
	v_lshlrev_b64 v[0:1], 3, v[86:87]
	v_add_co_u32 v0, s0, s10, v0
	v_add_co_ci_u32_e64 v1, s0, s11, v1, s0
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_17
.LBB0_20:                               ;   in Loop: Header=BB0_3 Depth=1
	s_or_b32 exec_lo, exec_lo, s26
	s_add_i32 s0, s1, 3
	v_mov_b32_e32 v2, v97
	v_mov_b32_e32 v3, v93
	s_cmp_lt_u32 s0, s16
	s_mov_b32 s26, 0
	s_cselect_b32 s27, -1, 0
	s_branch .LBB0_22
.LBB0_21:                               ;   in Loop: Header=BB0_22 Depth=2
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s28
	v_add_nc_u32_e32 v3, s19, v3
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v2, v[0:1]
	v_add_nc_u32_e32 v2, s21, v2
	v_cmp_lt_u32_e64 s0, 47, v3
	s_or_b32 s26, s0, s26
	s_andn2_b32 exec_lo, exec_lo, s26
	s_cbranch_execz .LBB0_24
.LBB0_22:                               ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_add_nc_u32_e32 v0, s3, v3
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s0, s7, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s0, s27, s0
	s_and_saveexec_b32 s28, s0
	s_cbranch_execz .LBB0_21
; %bb.23:                               ;   in Loop: Header=BB0_22 Depth=2
	v_add_nc_u32_e32 v86, s18, v3
	v_lshlrev_b64 v[0:1], 3, v[86:87]
	v_add_co_u32 v0, s0, s10, v0
	v_add_co_ci_u32_e64 v1, s0, s11, v1, s0
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_21
.LBB0_24:                               ;   in Loop: Header=BB0_3 Depth=1
	s_or_b32 exec_lo, exec_lo, s26
	s_add_i32 s0, s1, 4
	v_mov_b32_e32 v2, v98
	v_mov_b32_e32 v3, v93
	s_cmp_lt_u32 s0, s16
	s_mov_b32 s26, 0
	s_cselect_b32 s27, -1, 0
	s_branch .LBB0_26
.LBB0_25:                               ;   in Loop: Header=BB0_26 Depth=2
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s28
	v_add_nc_u32_e32 v3, s19, v3
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v2, v[0:1]
	v_add_nc_u32_e32 v2, s21, v2
	v_cmp_lt_u32_e64 s0, 47, v3
	s_or_b32 s26, s0, s26
	s_andn2_b32 exec_lo, exec_lo, s26
	s_cbranch_execz .LBB0_28
.LBB0_26:                               ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_add_nc_u32_e32 v0, s3, v3
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s0, s7, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s0, s27, s0
	s_and_saveexec_b32 s28, s0
	s_cbranch_execz .LBB0_25
; %bb.27:                               ;   in Loop: Header=BB0_26 Depth=2
	v_add_nc_u32_e32 v86, s20, v3
	v_lshlrev_b64 v[0:1], 3, v[86:87]
	v_add_co_u32 v0, s0, s10, v0
	v_add_co_ci_u32_e64 v1, s0, s11, v1, s0
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_25
.LBB0_28:                               ;   in Loop: Header=BB0_3 Depth=1
	s_or_b32 exec_lo, exec_lo, s26
	s_add_i32 s1, s1, 5
	v_mov_b32_e32 v2, v99
	v_mov_b32_e32 v3, v93
	s_cmp_lt_u32 s1, s16
	s_mov_b32 s1, 0
	s_cselect_b32 s26, -1, 0
	s_branch .LBB0_30
.LBB0_29:                               ;   in Loop: Header=BB0_30 Depth=2
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s27
	v_add_nc_u32_e32 v3, s19, v3
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v2, v[0:1]
	v_add_nc_u32_e32 v2, s21, v2
	v_cmp_lt_u32_e64 s0, 47, v3
	s_or_b32 s1, s0, s1
	s_andn2_b32 exec_lo, exec_lo, s1
	s_cbranch_execz .LBB0_2
.LBB0_30:                               ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_add_nc_u32_e32 v0, s3, v3
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s0, s7, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s0, s26, s0
	s_and_saveexec_b32 s27, s0
	s_cbranch_execz .LBB0_29
; %bb.31:                               ;   in Loop: Header=BB0_30 Depth=2
	v_add_nc_u32_e32 v86, s22, v3
	v_lshlrev_b64 v[0:1], 3, v[86:87]
	v_add_co_u32 v0, s0, s10, v0
	v_add_co_ci_u32_e64 v1, s0, s11, v1, s0
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_29
.LBB0_32:                               ; %Flow523
	v_add_nc_u32_e32 v3, s2, v88
	v_add_nc_u32_e32 v0, s3, v89
	v_mul_lo_u32 v4, s7, v3
	v_cmp_gt_u32_e64 s2, s14, v3
	v_cmp_gt_u32_e32 vcc_lo, s7, v0
	s_and_b32 s0, s2, vcc_lo
	s_and_saveexec_b32 s1, s0
	s_cbranch_execz .LBB0_34
; %bb.33:
	v_add_nc_u32_e32 v1, v4, v0
	v_mov_b32_e32 v2, 0
	v_lshlrev_b64 v[1:2], 3, v[1:2]
	v_add_co_u32 v1, s0, s12, v1
	v_add_co_ci_u32_e64 v2, s0, s13, v2, s0
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[1:2], v[84:85], off
.LBB0_34:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s1
	v_add_nc_u32_e32 v1, 1, v0
	v_cmp_gt_u32_e64 s0, s7, v1
	s_and_b32 s1, s2, s0
	s_and_saveexec_b32 s3, s1
	s_cbranch_execz .LBB0_36
; %bb.35:
	v_add_nc_u32_e32 v5, v4, v1
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s1, s12, v5
	v_add_co_ci_u32_e64 v6, s1, s13, v6, s1
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[82:83], off
.LBB0_36:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_add_nc_u32_e32 v2, 2, v0
	v_cmp_gt_u32_e64 s1, s7, v2
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execz .LBB0_38
; %bb.37:
	v_add_nc_u32_e32 v5, v4, v2
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s2, s12, v5
	v_add_co_ci_u32_e64 v6, s2, s13, v6, s2
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[80:81], off
.LBB0_38:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_or_b32_e32 v5, 1, v3
	v_add_nc_u32_e32 v4, s7, v4
	v_cmp_gt_u32_e64 s2, s14, v5
	s_and_b32 s3, s2, vcc_lo
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_66
; %bb.39:
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_67
.LBB0_40:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execz .LBB0_42
.LBB0_41:
	v_add_nc_u32_e32 v5, v4, v2
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s2, s12, v5
	v_add_co_ci_u32_e64 v6, s2, s13, v6, s2
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[74:75], off
.LBB0_42:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_or_b32_e32 v5, 2, v3
	v_add_nc_u32_e32 v4, s7, v4
	v_cmp_gt_u32_e64 s2, s14, v5
	s_and_b32 s3, s2, vcc_lo
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_68
; %bb.43:
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_69
.LBB0_44:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execz .LBB0_46
.LBB0_45:
	v_add_nc_u32_e32 v5, v4, v2
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s2, s12, v5
	v_add_co_ci_u32_e64 v6, s2, s13, v6, s2
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[68:69], off
.LBB0_46:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_or_b32_e32 v5, 3, v3
	v_add_nc_u32_e32 v4, s7, v4
	v_cmp_gt_u32_e64 s2, s14, v5
	s_and_b32 s3, s2, vcc_lo
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_70
; %bb.47:
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_71
.LBB0_48:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execz .LBB0_50
.LBB0_49:
	v_add_nc_u32_e32 v5, v4, v2
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s2, s12, v5
	v_add_co_ci_u32_e64 v6, s2, s13, v6, s2
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[62:63], off
.LBB0_50:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_or_b32_e32 v5, 4, v3
	v_add_nc_u32_e32 v4, s7, v4
	v_cmp_gt_u32_e64 s2, s14, v5
	s_and_b32 s3, s2, vcc_lo
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_72
; %bb.51:
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_73
.LBB0_52:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execz .LBB0_54
.LBB0_53:
	v_add_nc_u32_e32 v5, v4, v2
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s2, s12, v5
	v_add_co_ci_u32_e64 v6, s2, s13, v6, s2
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[56:57], off
.LBB0_54:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_or_b32_e32 v5, 5, v3
	v_add_nc_u32_e32 v4, s7, v4
	v_cmp_gt_u32_e64 s2, s14, v5
	s_and_b32 s3, s2, vcc_lo
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_74
; %bb.55:
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_75
.LBB0_56:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execz .LBB0_58
.LBB0_57:
	v_add_nc_u32_e32 v5, v4, v2
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s2, s12, v5
	v_add_co_ci_u32_e64 v6, s2, s13, v6, s2
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[50:51], off
.LBB0_58:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_or_b32_e32 v5, 6, v3
	v_add_nc_u32_e32 v4, s7, v4
	v_cmp_gt_u32_e64 s2, s14, v5
	s_and_b32 s3, s2, vcc_lo
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_76
; %bb.59:
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_77
.LBB0_60:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execz .LBB0_62
.LBB0_61:
	v_add_nc_u32_e32 v5, v4, v2
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s2, s12, v5
	v_add_co_ci_u32_e64 v6, s2, s13, v6, s2
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[44:45], off
.LBB0_62:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_or_b32_e32 v3, 7, v3
	v_cmp_gt_u32_e64 s2, s14, v3
	v_add_nc_u32_e32 v3, s7, v4
	s_and_b32 s4, s2, vcc_lo
	s_and_saveexec_b32 s3, s4
	s_cbranch_execnz .LBB0_78
; %bb.63:
	s_or_b32 exec_lo, exec_lo, s3
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s0, s3
	s_cbranch_execnz .LBB0_79
.LBB0_64:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s0
	s_and_b32 s0, s2, s1
	s_and_saveexec_b32 s1, s0
	s_cbranch_execnz .LBB0_80
.LBB0_65:
	s_endpgm
.LBB0_66:
	v_add_nc_u32_e32 v5, v4, v0
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[78:79], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execz .LBB0_40
.LBB0_67:
	v_add_nc_u32_e32 v5, v4, v1
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[76:77], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execnz .LBB0_41
	s_branch .LBB0_42
.LBB0_68:
	v_add_nc_u32_e32 v5, v4, v0
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[72:73], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execz .LBB0_44
.LBB0_69:
	v_add_nc_u32_e32 v5, v4, v1
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[70:71], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execnz .LBB0_45
	s_branch .LBB0_46
.LBB0_70:
	v_add_nc_u32_e32 v5, v4, v0
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[66:67], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execz .LBB0_48
.LBB0_71:
	v_add_nc_u32_e32 v5, v4, v1
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[64:65], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execnz .LBB0_49
	s_branch .LBB0_50
.LBB0_72:
	v_add_nc_u32_e32 v5, v4, v0
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[60:61], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execz .LBB0_52
.LBB0_73:
	v_add_nc_u32_e32 v5, v4, v1
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[58:59], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execnz .LBB0_53
	s_branch .LBB0_54
.LBB0_74:
	v_add_nc_u32_e32 v5, v4, v0
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[54:55], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execz .LBB0_56
.LBB0_75:
	v_add_nc_u32_e32 v5, v4, v1
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[52:53], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execnz .LBB0_57
	s_branch .LBB0_58
.LBB0_76:
	v_add_nc_u32_e32 v5, v4, v0
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[48:49], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execz .LBB0_60
.LBB0_77:
	v_add_nc_u32_e32 v5, v4, v1
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[46:47], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execnz .LBB0_61
	s_branch .LBB0_62
.LBB0_78:
	v_add_nc_u32_e32 v4, v3, v0
	v_mov_b32_e32 v5, 0
	v_lshlrev_b64 v[4:5], 3, v[4:5]
	v_add_co_u32 v4, vcc_lo, s12, v4
	v_add_co_ci_u32_e32 v5, vcc_lo, s13, v5, vcc_lo
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[4:5], v[42:43], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s0, s3
	s_cbranch_execz .LBB0_64
.LBB0_79:
	v_add_nc_u32_e32 v0, v3, v1
	v_mov_b32_e32 v1, 0
	v_lshlrev_b64 v[0:1], 3, v[0:1]
	v_add_co_u32 v0, vcc_lo, s12, v0
	v_add_co_ci_u32_e32 v1, vcc_lo, s13, v1, vcc_lo
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[0:1], v[40:41], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s0
	s_and_b32 s0, s2, s1
	s_and_saveexec_b32 s1, s0
	s_cbranch_execz .LBB0_65
.LBB0_80:
	v_add_nc_u32_e32 v0, v3, v2
	v_mov_b32_e32 v1, 0
	v_lshlrev_b64 v[0:1], 3, v[0:1]
	v_add_co_u32 v0, vcc_lo, s12, v0
	v_add_co_ci_u32_e32 v1, vcc_lo, s13, v1, vcc_lo
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[0:1], v[38:39], off
	s_endpgm
	.section	.rodata,"a",@progbits
	.p2align	6, 0x0
	.amdhsa_kernel mul
		.amdhsa_group_segment_fixed_size 2688
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
		.amdhsa_next_free_vgpr 104
		.amdhsa_next_free_sgpr 29
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
; codeLenInByte = 5524
; NumSgprs: 31
; NumVgprs: 104
; ScratchSize: 0
; MemoryBound: 0
; FloatMode: 240
; IeeeMode: 1
; LDSByteSize: 2688 bytes/workgroup (compile time only)
; SGPRBlocks: 3
; VGPRBlocks: 12
; NumSGPRsForWavesPerEU: 31
; NumVGPRsForWavesPerEU: 104
; Occupancy: 9
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
    .group_segment_fixed_size: 2688
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
    .sgpr_count:     31
    .sgpr_spill_count: 0
    .symbol:         mul.kd
    .uniform_work_group_size: 1
    .uses_dynamic_stack: false
    .vgpr_count:     104
    .vgpr_spill_count: 0
    .wavefront_size: 32
    .workgroup_processor_mode: 1
amdhsa.target:   'amdgcn-amd-amdhsa--gfx1010:xnack-'
amdhsa.version:
  - 1
  - 2
...

	.end_amdgpu_metadata
