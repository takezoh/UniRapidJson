// Copyright (c) 2017 Takehito Gondo
//
// UniRapidJson is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#include "platform/platform-env.h"
#include "rapidjson/document.h"
#include "rapidjson/pointer.h"
#include "rapidjson/filereadstream.h"
#include "cstype.h"

using namespace rapidjson;
using namespace cstype;


namespace {
	template <typename TStream>
	const Document* _gendoc(TStream& stream) {
		// dont use in-situ.
		auto doc = new Document();
		doc->ParseStream(stream);
		if (doc->HasParseError()) {
			delete doc;
			return nullptr;
		}
		return doc;
	}
}

extern "C" {

	DLLEXPORT BOOL _rapidjson_new_document_from_memory_string(STRING json, INTPTR<Document>::CP *document) {
		StringStream ss(json);
		*document = _gendoc(ss);
		return *document != nullptr;
	}

	DLLEXPORT BOOL _rapidjson_new_document_from_memory_bytes(BYTE *bytes, SIZE length, INTPTR<Document>::CP *document) {
		if (length > 0 && bytes[length - 1] == '\0') {
			return _rapidjson_new_document_from_memory_string((const char *)bytes, document);
		}
		MemoryStream ss((char*)bytes, length);
		*document = _gendoc(ss);
		return *document != nullptr;
	}

	DLLEXPORT BOOL _rapidjson_new_document_from_file(STRING filepath, INTPTR<Document>::CP *document) {
		*document = nullptr;
#ifdef JSON_WINDOWS
		FILE* fp = nullptr;
		fopen_s(&fp, filepath, "rb");
#else
		FILE* fp = fopen(filepath, "r");
#endif
		if (fp) {
			char readBuffer[65536];
			FileReadStream is(fp, readBuffer, sizeof(readBuffer));
			*document = _gendoc(is);
			fclose(fp);
		}
		return *document != nullptr;
	}

	DLLEXPORT void _rapidjson_delete_document(INTPTR<Document>::P *document) {
		if (document && *document) {
			delete *document;
			*document = nullptr;
		}
	}

	DLLEXPORT BOOL _rapidjson_get_value_by_object(INTPTR<Value>::CP src, STRING name, BOOL *isObject, INTPTR<Value>::CP *dst) {
		if (src && (*isObject = src->IsObject())) {
			auto itr = src->FindMember(name);
			if (itr != src->MemberEnd()) {
				*dst = &itr->value;
				return true;
			}
		}
		return false;
	}

	DLLEXPORT BOOL _rapidjson_get_object_member_count(INTPTR<Value>::CP src, SIZE *size) {
		if (src && src->IsObject()) {
			*size = src->MemberCount();
			return true;
		}
		return false;
	}

	DLLEXPORT BOOL _rapidjson_get_key_value_pair_by_object_index(INTPTR<Value>::CP src, SIZE index, STRING *key, INTPTR<Value>::CP *value) {
		if (src) {
			auto member = src->MemberBegin() + index;
			auto size = member->name.GetStringLength() + 1;
			auto name = (char*)malloc(sizeof(char) * size);
#ifdef JSON_WINDOWS
			strcpy_s(name, size, member->name.GetString());
#else
			strcpy(name, member->name.GetString());
#endif
			*key = name;
			*value = &member->value;
			return true;
		}
		return false;
	}

	DLLEXPORT BOOL _rapidjson_get_value_by_array(INTPTR<Value>::CP src, SIZE index, BOOL *isArray, INTPTR<Value>::CP *dst) {
		if (src && (*isArray = src->IsArray())) {
			if ((unsigned int)index < src->Size()) {
				*dst = src->Begin() + index;
				return true;
			}
		}
		return false;
	}

	DLLEXPORT BOOL _rapidjson_get_array_iterator(INTPTR<Value>::CP src, INTPTR<Value>::CP *elementsPointer, SIZE *size, SIZE *elementSize) {
		if (src && src->IsArray()) {
			*elementsPointer = src->Begin();
			*size = src->Size();
			*elementSize = sizeof(const Value);
			return true;
		}
		return false;
	}

	DLLEXPORT BOOL _rapidjson_get_array_size(INTPTR<Value>::CP src, SIZE *dst) {
		if (src && src->IsArray()) {
			*dst = src->Size();
			return true;
		}
		return false;
	}

	DLLEXPORT BOOL _rapidjson_get_int(INTPTR<Value>::CP src, INT *dst) {
		if (src && src->IsInt()) {
			*dst = src->GetInt();
			return true;
		}
		return false;
	}

	DLLEXPORT BOOL _rapidjson_get_uint(INTPTR<Value>::CP src, UINT *dst) {
		if (src && src->IsUint()) {
			*dst = src->GetUint();
			return true;
		}
		return false;
	}

	DLLEXPORT BOOL _rapidjson_get_int64(INTPTR<Value>::CP src, LONG *dst) {
		if (src && src->IsInt64()) {
			*dst = src->GetInt64();
			return true;
		}
		return false;
	}

	DLLEXPORT BOOL _rapidjson_get_uint64(INTPTR<Value>::CP src, ULONG *dst) {
		if (src && src->IsUint64()) {
			*dst = src->GetUint64();
			return true;
		}
		return false;
	}

	DLLEXPORT BOOL _rapidjson_get_float(INTPTR<Value>::CP src, FLOAT *dst) {
		if (src) {
			if (src->IsFloat()) {
				*dst = src->GetFloat();
				return true;
			}
			// if (src->IsInt()) {
			// 	*dst = static_cast<float>(src->GetInt());
			// 	return true;
			// }
		}
		return false;
	}

	DLLEXPORT BOOL _rapidjson_get_double(INTPTR<Value>::CP src, DOUBLE *dst) {
		if (src) {
			if (src->IsDouble()) {
				*dst = src->GetDouble();
				return true;
			}
			// if (src->IsInt64()) {
			// 	*dst = static_cast<double>(src->GetInt64());
			// 	return true;
			// }
		}
		return false;
	}

	DLLEXPORT BOOL _rapidjson_get_string(INTPTR<Value>::CP src, STRING *dst) {
		if (src) {
			if (src->IsString()) {
				int size = src->GetStringLength() + 1;
				auto str = (char*)malloc(sizeof(char) * size);
#ifdef JSON_WINDOWS
				strcpy_s(str, size, src->GetString());
#else
				strcpy(str, src->GetString());
#endif
				*dst = str;
				return true;
			}
			// if (src->IsNull()) {
			// 	*dst = nullptr;
			// 	return true;
			// }
		}
		return false;
	}

	DLLEXPORT BOOL _rapidjson_get_bool(INTPTR<Value>::CP src, BOOL *dst) {
		if (src && src->IsBool()) {
			*dst = src->GetBool();
			return true;
		}
		return false;
	}

	DLLEXPORT BOOL _rapidjson_get_type(INTPTR<Value>::CP src, INT *dst) {
		if (src) {
			*dst = (int)src->GetType();
			return true;
		}
		return false;
	}

	DLLEXPORT BOOL _rapidjson_is_array(INTPTR<Value>::CP src) {
		return src && src->IsArray();
	}

	DLLEXPORT BOOL _rapidjson_is_object(INTPTR<Value>::CP src) {
		return src && src->IsObject();
	}

	DLLEXPORT BOOL _rapidjson_is_number(INTPTR<Value>::CP src) {
		return src && src->IsNumber();
	}

	DLLEXPORT BOOL _rapidjson_is_int(INTPTR<Value>::CP src) {
		return src && src->IsInt();
	}

	DLLEXPORT BOOL _rapidjson_is_uint(INTPTR<Value>::CP src) {
		return src && src->IsUint();
	}

	DLLEXPORT BOOL _rapidjson_is_int64(INTPTR<Value>::CP src) {
		return src && src->IsInt64();
	}

	DLLEXPORT BOOL _rapidjson_is_uint64(INTPTR<Value>::CP src) {
		return src && src->IsUint64();
	}

	DLLEXPORT BOOL _rapidjson_is_float(INTPTR<Value>::CP src) {
		// return src && (src->IsFloat() || src->IsInt());
		return src && src->IsFloat();
	}

	DLLEXPORT BOOL _rapidjson_is_double(INTPTR<Value>::CP src) {
		// return src && (src->IsDouble() || src->IsInt64());
		return src && src->IsDouble();
	}

	DLLEXPORT BOOL _rapidjson_is_string(INTPTR<Value>::CP src) {
		// return src && (src->IsString() || src->IsNull());
		return src && src->IsString();
	}

	DLLEXPORT BOOL _rapidjson_is_bool(INTPTR<Value>::CP src) {
		return src && src->IsBool();
	}

	DLLEXPORT BOOL _rapidjson_is_null(INTPTR<Value>::CP src) {
		return src && src->IsNull();
	}

	DLLEXPORT BOOL _rapidjson_get_value_by_pointer(INTPTR<Value>::CP src, STRING pointer, SIZE pointerLength, BOOL *isValid, INTPTR<Value>::CP *dst) {
		Pointer p(pointer, pointerLength);
		if (*isValid = p.IsValid()) {
			return (*dst = p.Get(*src, 0)) != nullptr;
		}
		return false;
	}
}
