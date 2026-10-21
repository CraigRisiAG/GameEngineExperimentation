
#include "resource_importer_csv.h"

#include "core/io/resource_saver.h"
#include "core/os/file_access.h"

String ResourceImporterCSV::get_importer_name() const { return "csv"; }

String ResourceImporterCSV::get_visible_name() const { return "CSV"; }
void ResourceImporterCSV::get_recognized_extensions(
    List<String> *p_extensions) const {

  p_extensions->push_back("csv");
}

String ResourceImporterCSV::get_save_extension() const {
  return ""; // does not save a single resource
}

String ResourceImporterCSV::get_resource_type() const { return "TextFile"; }

bool ResourceImporterCSV::get_option_visibility(
    const String &p_option, const Map<StringName, Variant> &p_options) const {

  return true;
}

int ResourceImporterCSV::get_preset_count() const { return 0; }
String ResourceImporterCSV::get_preset_name(int p_idx) const { return ""; }

void ResourceImporterCSV::get_import_options(List<ImportOption> *r_options,
                                             int p_preset) const {}

Error ResourceImporterCSV::import(const String &p_source_file,
                                  const String &p_save_path,
                                  const Map<StringName, Variant> &p_options,
                                  List<String> *r_platform_variants,
                                  List<String> *r_gen_files,
                                  Variant *r_metadata) {
  return OK;
}

ResourceImporterCSV::ResourceImporterCSV() {}
