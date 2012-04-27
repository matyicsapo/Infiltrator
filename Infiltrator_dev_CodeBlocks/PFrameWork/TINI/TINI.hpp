#ifndef TINI_HPP
#define TINI_HPP

#include <string>
#include <fstream>

namespace TINI {
	class TINIObject {
		protected:
			std::fstream file;
			std::string fileName;

		public:
			TINIObject ();
			TINIObject (std::string fileName);
			~TINIObject ();

			void NewFile (std::string fileName);
			void SetFile (std::string fileName);

			void Begin ();
			void End ();

			void SetValue (std::string section, std::string key, std::string value);

			std::string GetValue (std::string section, std::string key);

			void RemoveEntry (std::string section, std::string key);
	};
}

#endif // TINI_HPP
