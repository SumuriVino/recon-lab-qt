#include "z_mainwindow/mainwindow.h"

void MainWindow::update_result_necessity_for_data()
{
    recon_static_functions::app_debug(" start ", Q_FUNC_INFO);

    if(!bool_need_to_update_result_for_load_case)
        return;


    QString version_number_of_result = global_narad_muni_class_obj->get_field(MACRO_NARAD_Result_App_Version_QString).toString().trimmed();


    if(version_number_of_result.isEmpty())
        return;

    //========================================================================//
    if(version_number_of_result == "1.0.6")
    {
        update_result_106_to_107();
        update_result_107_to_109();
        update_result_109_to_111();
        update_result_111_to_112();
        update_result_112_to_114();
        update_result_114_to_116();
        update_result_116_to_117();
        update_result_117_to_120();
        update_result_123_to_125();
        update_result_125_to_126();
        update_result_126_to_128();
        update_result_128_to_129();
        update_result_129_to_130();
        update_result_130_to_132();
        update_result_132_to_133();
        update_result_133_to_135();
        update_result_135_to_136();
        update_result_136_to_137();
        update_result_137_to_138();
        update_result_138_to_139();
        update_result_139_to_140();
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.0.7" || version_number_of_result == "1.0.8" || version_number_of_result == "1.0.8.1")
    {
        update_result_107_to_109();
        update_result_109_to_111();
        update_result_111_to_112();
        update_result_112_to_114();
        update_result_114_to_116();
        update_result_116_to_117();
        update_result_117_to_120();
        update_result_120_to_123();
        update_result_123_to_125();
        update_result_125_to_126();
        update_result_126_to_128();
        update_result_128_to_129();
        update_result_129_to_130();
        update_result_130_to_132();
        update_result_132_to_133();
        update_result_133_to_135();
        update_result_135_to_136();
        update_result_136_to_137();
        update_result_137_to_138();
        update_result_138_to_139();
        update_result_139_to_140();
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();

    }
    else if(version_number_of_result == "1.0.9" || version_number_of_result == "1.1.0")
    {
        update_result_109_to_111();
        update_result_111_to_112();
        update_result_112_to_114();
        update_result_114_to_116();
        update_result_116_to_117();
        update_result_117_to_120();
        update_result_120_to_123();
        update_result_123_to_125();
        update_result_125_to_126();
        update_result_126_to_128();
        update_result_128_to_129();
        update_result_129_to_130();
        update_result_130_to_132();
        update_result_132_to_133();
        update_result_133_to_135();
        update_result_135_to_136();
        update_result_136_to_137();
        update_result_137_to_138();
        update_result_138_to_139();
        update_result_139_to_140();
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.1.1")
    {
        update_result_111_to_112();
        update_result_112_to_114();
        update_result_114_to_116();
        update_result_116_to_117();
        update_result_117_to_120();
        update_result_120_to_123();
        update_result_123_to_125();
        update_result_125_to_126();
        update_result_126_to_128();
        update_result_128_to_129();
        update_result_129_to_130();
        update_result_130_to_132();
        update_result_132_to_133();
        update_result_133_to_135();
        update_result_135_to_136();
        update_result_136_to_137();
        update_result_137_to_138();
        update_result_138_to_139();
        update_result_139_to_140();
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();

        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.1.2" || version_number_of_result == "1.1.3")
    {
        update_result_112_to_114();
        update_result_114_to_116();
        update_result_116_to_117();
        update_result_117_to_120();
        update_result_120_to_123();
        update_result_123_to_125();
        update_result_125_to_126();
        update_result_126_to_128();
        update_result_128_to_129();
        update_result_129_to_130();
        update_result_130_to_132();
        update_result_132_to_133();
        update_result_133_to_135();
        update_result_135_to_136();
        update_result_136_to_137();
        update_result_137_to_138();
        update_result_138_to_139();
        update_result_139_to_140();
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();

        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.1.4" || version_number_of_result == "1.1.5")
    {
        update_result_114_to_116();
        update_result_116_to_117();
        update_result_117_to_120();
        update_result_120_to_123();
        update_result_123_to_125();
        update_result_125_to_126();
        update_result_126_to_128();
        update_result_128_to_129();
        update_result_129_to_130();
        update_result_130_to_132();
        update_result_132_to_133();
        update_result_133_to_135();
        update_result_135_to_136();
        update_result_136_to_137();
        update_result_137_to_138();
        update_result_138_to_139();
        update_result_139_to_140();
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();

        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.1.6")
    {
        update_result_116_to_117();
        update_result_117_to_120();
        update_result_120_to_123();
        update_result_123_to_125();
        update_result_125_to_126();
        update_result_126_to_128();
        update_result_128_to_129();
        update_result_129_to_130();
        update_result_130_to_132();
        update_result_132_to_133();
        update_result_133_to_135();
        update_result_135_to_136();
        update_result_136_to_137();
        update_result_137_to_138();
        update_result_138_to_139();
        update_result_139_to_140();
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();

        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.1.7" || version_number_of_result == "1.1.8" || version_number_of_result == "1.1.9")
    {
        update_result_117_to_120();
        update_result_120_to_123();
        update_result_123_to_125();
        update_result_125_to_126();
        update_result_126_to_128();
        update_result_128_to_129();
        update_result_129_to_130();
        update_result_130_to_132();
        update_result_132_to_133();
        update_result_133_to_135();
        update_result_135_to_136();
        update_result_136_to_137();
        update_result_137_to_138();
        update_result_138_to_139();
        update_result_139_to_140();
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
   }
    else if(version_number_of_result == "1.2.0" || version_number_of_result == "1.2.1" || version_number_of_result == "1.2.2")
    {
        update_result_120_to_123();
        update_result_123_to_125();
        update_result_125_to_126();
        update_result_126_to_128();
        update_result_128_to_129();
        update_result_129_to_130();
        update_result_130_to_132();
        update_result_132_to_133();
        update_result_133_to_135();
        update_result_135_to_136();
        update_result_136_to_137();
        update_result_137_to_138();
        update_result_138_to_139();
        update_result_139_to_140();
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.2.3" || version_number_of_result == "1.2.4")
    {
        update_result_123_to_125();
        update_result_125_to_126();
        update_result_126_to_128();
        update_result_128_to_129();
        update_result_129_to_130();
        update_result_130_to_132();
        update_result_132_to_133();
        update_result_133_to_135();
        update_result_135_to_136();
        update_result_136_to_137();
        update_result_137_to_138();
        update_result_138_to_139();
        update_result_139_to_140();
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
   }
    else if(version_number_of_result == "1.2.5")
    {
        update_result_125_to_126();
        update_result_126_to_128();
        update_result_128_to_129();
        update_result_129_to_130();
        update_result_130_to_132();
        update_result_132_to_133();
        update_result_133_to_135();
        update_result_135_to_136();
        update_result_136_to_137();
        update_result_137_to_138();
        update_result_138_to_139();
        update_result_139_to_140();
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.2.6" || version_number_of_result == "1.2.7")
    {
        update_result_126_to_128();
        update_result_128_to_129();
        update_result_129_to_130();
        update_result_130_to_132();
        update_result_132_to_133();
        update_result_133_to_135();
        update_result_135_to_136();
        update_result_136_to_137();
        update_result_137_to_138();
        update_result_138_to_139();
        update_result_139_to_140();
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.2.8")
    {
        update_result_128_to_129();
        update_result_129_to_130();
        update_result_130_to_132();
        update_result_132_to_133();
        update_result_133_to_135();
        update_result_135_to_136();
        update_result_136_to_137();
        update_result_137_to_138();
        update_result_138_to_139();
        update_result_139_to_140();
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.2.9")
    {
        update_result_129_to_130();
        update_result_130_to_132();
        update_result_132_to_133();
        update_result_133_to_135();
        update_result_135_to_136();
        update_result_136_to_137();
        update_result_137_to_138();
        update_result_138_to_139();
        update_result_139_to_140();
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.3.0" || version_number_of_result == "1.3.1")
    {
        update_result_130_to_132();
        update_result_132_to_133();
        update_result_133_to_135();
        update_result_135_to_136();
        update_result_136_to_137();
        update_result_137_to_138();
        update_result_138_to_139();
        update_result_139_to_140();
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.3.2")
    {
        update_result_132_to_133();
        update_result_133_to_135();
        update_result_135_to_136();
        update_result_136_to_137();
        update_result_137_to_138();
        update_result_138_to_139();
        update_result_139_to_140();
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.3.3" || version_number_of_result == "1.3.4")
    {
        update_result_133_to_135();
        update_result_135_to_136();
        update_result_136_to_137();
        update_result_137_to_138();
        update_result_138_to_139();
        update_result_139_to_140();
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.3.5")
    {
        update_result_135_to_136();
        update_result_136_to_137();
        update_result_137_to_138();
        update_result_138_to_139();
        update_result_139_to_140();
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.3.6")
    {
        update_result_136_to_137();
        update_result_137_to_138();
        update_result_138_to_139();
        update_result_139_to_140();
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.3.7")
    {
        update_result_137_to_138();
        update_result_138_to_139();
        update_result_139_to_140();
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.3.8")
    {
        update_result_138_to_139();
        update_result_139_to_140();
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.3.9")
    {
        update_result_139_to_140();
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.4.0")
    {
        update_result_140_to_141();
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.4.1")
    {
        update_result_141_to_142();
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.4.2")
    {
        update_result_142_to_143();
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.4.3")
    {
        update_result_143_to_144();
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.4.4")
    {
        update_result_144_to_145();
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
   }
    else if(version_number_of_result == "1.4.5")
    {
        update_result_145_to_146();
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.4.6")
    {
        update_result_146_to_147();
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.4.7")
    {
        update_result_147_to_148();
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.4.8" || version_number_of_result == "1.4.9")
    {
        update_result_148_to_150();
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.5.0")
    {
        update_result_150_to_151();
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.5.1")
    {
        update_result_151_to_152();
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.5.2")
    {
        update_result_152_to_153();
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.5.3")
    {
        update_result_153_to_154();
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.5.4")
    {
        update_result_154_to_155();
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.5.5" || version_number_of_result == "1.5.6")
    {
        update_result_155_to_157();
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.5.7")
    {
        update_result_157_to_158();
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.5.8")
    {
        update_result_158_to_159();
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.5.9")
    {
        update_result_159_to_160();
        update_result_160_to_161();
    }
    else if(version_number_of_result == "1.6.0")
    {
        update_result_160_to_161();
    }

    recon_static_functions::app_debug(" end ", Q_FUNC_INFO);

}
