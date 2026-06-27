package com.nannan.nansu.data.repository

import com.nannan.nansu.ui.viewmodel.KpmViewModel

interface KpmRepository {
    suspend fun getModuleList(): Result<List<KpmViewModel.ModuleInfo>>
    suspend fun getModuleInfo(moduleId: String): Result<String>
    suspend fun loadModule(path: String, args: String? = null): Result<Unit>
    suspend fun unloadModule(moduleId: String): Result<Unit>
    suspend fun controlModule(moduleId: String, args: String? = null): Result<Int>
}
