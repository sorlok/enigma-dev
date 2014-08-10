/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**  Copyright (C) 2014 Seth N. Hetu                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

//Welcome to the ENIGMA EDL-to-C++ parser; just add semicolons.
//...No, it's not really that simple.

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
using namespace std;

#include "config.h"
#include "compiler/event_reader/event_parser.h"

extern int global_script_argument_count;

//Tracks state variables for the current scope.
struct scope_level {
  map<string,int> ignore;  //List of ignored symbols for this scope.
  bool is_with; //Did this or any previous scope represent a "with" block? If so, we need to tag variables.
  bool no_bracket; //If true, this is a "single-line" scope (no brackets).
  
//  scope_level(scope_ignore*x): is_with(x->is_with) {}
  scope_level(bool is_with=false): is_with(is_with), no_bracket(false) {}
//  scope_level(int is_with): is_with(is_with) {}
};

#include "object_storage.h"
#include "collect_variables.h"
#include "languages/language_adapter.h"

void collect_variables(language_adapter *lang, string &code, string &synt, parsed_event* pev, const std::set<std::string>& script_names)
{
//  int igpos = 0;
  std::vector<scope_level> scopes; //.back() is the current scope level.
  std::pair<bool, scope_level> reify_scope; //The last popped "if" scope, to be restored if an "else" is encountered.
  reify_scope.first = false; //Not currently holding anything.
  scopes.push_back(scope_level(false));
//  igstack[igpos] = new scope_ignore(0);
  
  cout << "\nCollecting some variables...\n";

std::cerr <<"CODE: ###" <<code <<"###\n";
std::cerr <<"SYNT: ###" <<synt <<"###\n";

  pt dec_start_pos = 0;
  
  int in_decl = 0, dec_out_of_scope = 0; //Not declaring, not declaring outside this scope via global or local
  string dec_prefixes, dec_type, dec_suffixes; //Unary referencers, type names and flags, and unary-postfix array subscripts
  int dec_initializing = false; //This tells us we've passed an = in our initialization
  pt dec_equals_at = 0;
  int bracklevel = 0; //This will help us make sure not to add unwanted unary symbols or miss variables/scripts.
  
  bool dec_name_givn = false; //Has an identifier been named in this declaration?
  string dec_name; //Identifier being declared
  
  int  counting_parens = 0; //How many parentheses are after the current if/with statement are we at? 
  
//bool with_until_semi = false;
  bool last_was_with = false; //Was the last token parsed "with"?
  bool skip_next_bracket = false; //If true, with(x), if(x), or similar have just pushed a scope to the stack. The very next bracket should be ignored.

  bool grab_tline_index = false; //Are we currently trying to stockpile a list of known timeline indices?
  
  for (pt pos = 0; pos < code.length(); pos++)
  {
    //Stop grabbing the timeline index?
    if (grab_tline_index) {
      grab_tline_index = (synt[pos]=='n') || (synt[pos]=='=') || (synt[pos]=='(');
    }

    if (synt[pos] == '{') {
      //New scope. Inherit "is_with" from previous scope, or if we've just passed a "with" statement.
      if (!skip_next_bracket) {
        scopes.push_back(scope_level(scopes.back().is_with));
      }
      skip_next_bracket = false;
//      bool isw = scopes.back().is_with || with_until_semi; 
//      with_until_semi = 0;
//      igstack[++igpos] = new scope_ignore(isw);
      continue;
    }
    if (synt[pos] == '}') {
      if (pos+1<synt.size() && synt[pos+1]=='e') {
        reify_scope.first = true;
        reify_scope.second = scopes.back();
      }

      scopes.pop_back();//[igpos--];

      //Pop more scopes?
      if (pos+1<synt.size() && synt[pos+1]!='e') {
        while (scopes.back().no_bracket) {
          scopes.pop_back();
        }
      }

      continue;
    }
    
    //Count parentheses, if we've just passed a with/if statement.
    if (synt[pos] == '(' and counting_parens>0) {
      counting_parens++;
    }
    if (synt[pos] == ')' and counting_parens>0) {
      if (--counting_parens == 1) {
        counting_parens = 0;

        //We may be reifying an old scope.
        if (reify_scope.first) {
          scopes.push_back(reify_scope.second);
          reify_scope.first = false;
        } else {
          scopes.push_back(scope_level(scopes.back().is_with || last_was_with));
          last_was_with = false;
        }

        //Regardless, set the bracket state:
        skip_next_bracket = pos+1<synt.size() && synt[pos+1]=='{';
        scopes.back().no_bracket = !skip_next_bracket;

        //scopes.back().no_bracket = pos==synt.size()-1 || synt[pos]!='{';
        //with_until_semi = with_after_parenths--;
      }
    }

    //End scope if a semicolon is encountered.
    if (synt[pos]==';' && scopes.back().no_bracket) {
      //...but save it if an "else" or "elseif" is next.
      if (pos+1<synt.size() && synt[pos+1]=='e') {
        reify_scope.first = true;
        reify_scope.second = scopes.back();
      }
      scopes.pop_back();

      //Pop more scopes?
      if (pos+1<synt.size() && synt[pos+1]!='e') {
        while (scopes.back().no_bracket) {
          scopes.pop_back();
        }
      }
    }
    
    //with_until_semi &= (synt[pos] != ';');

//////////////////////////////////////////////////////////    
//TODO: I am still not familiar with what this code does. This will have to be addressed before a pull request is made.
//////////////////////////////////////////////////////////
    if (bracklevel == 0 and in_decl)
    {
      if (synt[pos] == ';' or synt[pos] == ',')
      {
        const bool was_a_semicolon = synt[pos] == ';';
        
        if (dec_name_givn)
        {
          if (dec_out_of_scope) //Designated for a different scope: global or local
          {
            //Declare this as a specific type
            cout << "Declared " << dec_type << " " << dec_prefixes << dec_name << dec_suffixes << " as " << (dec_out_of_scope-1 ? "global" : "local") << endl;
            if (dec_out_of_scope - 1) //to be placed at global scope
              pev->myObj->globals[dec_name] = dectrip(dec_type,dec_prefixes,dec_suffixes);
            else
              pev->myObj->locals[dec_name] = dectrip(dec_type,dec_prefixes,dec_suffixes);

            if (!dec_initializing) //If this statement does nothing other than declare, remove it
            {
              cout << "ERASE FROM CODE: " << code.substr(dec_start_pos,pos+1-dec_start_pos) << endl;
              code.erase(dec_start_pos,pos+1-dec_start_pos);
              synt.erase(dec_start_pos,pos+1-dec_start_pos);
              pos = dec_start_pos;
            }
            else
            {
              cout << "ERASE FROM CODE: " << code.substr(dec_start_pos,dec_equals_at-dec_start_pos) << endl;
              code.replace(dec_start_pos,dec_equals_at-dec_start_pos, dec_name);
              synt.replace(dec_start_pos,dec_equals_at-dec_start_pos, string(dec_name.length(),'n'));
              pos -= dec_equals_at - dec_start_pos - 1 - dec_name.length();
            }
            dec_start_pos = pos;
          }
          else //Add to this scope
          {
            scopes.back().ignore[dec_name] = pos;
            pos++; //cout << "Added `" << dec_name << "' to ig\n";
          }
        }
        
        if (was_a_semicolon)
          dec_out_of_scope = in_decl = 0, dec_type = "";
        
        dec_prefixes = dec_suffixes = "";
        dec_initializing = false;
        dec_name_givn = false;
      }
      if (!dec_initializing)
      {
        if (synt[pos] == '*') {
          dec_prefixes += "*";
          continue;
        }
        if (synt[pos] == '&') {
          dec_prefixes += "&";
          continue;
        }
        if (synt[pos] == '=') {
          dec_initializing = true;
          dec_equals_at = pos;
          continue;
        }
        if (synt[pos] == '(') {
          ((dec_name_givn)?dec_suffixes:dec_prefixes) += '(';
          continue;
        }
        if (synt[pos] == '(') {
          ((dec_name_givn)?dec_suffixes:dec_prefixes) += ')';
          continue;
        }
      }
    }
    if (synt[pos] == '[')
    {
      if (in_decl and !dec_initializing)
      {
        pt ep = pos + 1;
        for (int bc = 1; bc > 0; ep++)
          if (synt[ep] == '[') bc++;
          else if (synt[ep] == ']') bc--;
        dec_suffixes += code.substr(pos,ep - pos);
      }
      bracklevel++;
      continue;
    }
    if (synt[pos] == ']') {
      bracklevel--;
      continue;
    }
    
    if (synt[pos] == 'L') //Double meaning.
    {
      //Bookmark this spot
      const int sp = pos;
      
      //Determine which meaning it is.
      pos += 5; //Skip "L-O-C-A-L" or "G-L-O-B-A"
      if (synt[pos] == 'L') pos++;
      if (synt[pos] != 't')
      {
        for (pt i = sp; i < pos; i++)
          synt[i] = 'n'; //convert to regular identifier; in this case marking a constant.
        continue;
      }
      //We're at either global declarator or local declarator: record which scope it is.
      dec_out_of_scope = 1 + (code[sp] == 'g'); //If the first character of this token is 'g', it's global. Otherwise we assume it's local.
      
      //Remove the keyword from the code
      code.erase(sp,pos-sp);
      synt.erase(sp,pos-sp);
      pos = sp;
      
      fflush(stdout);
      goto past_this_if;
    }
    if (synt[pos] == 't')
    {
      past_this_if:
      
      //Skip to the end of the typename, remember where we started
      const int tsp = pos;
      while (synt[++pos] == 't');
      
      //Copy the type
      dec_type = code.substr(tsp,pos-tsp);
      
      if (dec_out_of_scope)
      {
        //Remove the keyword from the code
        code.erase(tsp,pos-tsp);
        synt.erase(tsp,pos-tsp);
        pos = tsp;
      }
      
      // Indicate that we're in a declaration and should start
      // Ignoring shit rather than adding it to the local list
      in_decl = true;
      
      // Log this position
      dec_start_pos = pos--;
      continue;
    }
    if (synt[pos] == 'n')
    {
      bool nts = !pos or synt[pos-1] != '.';
      
      const pt spos = pos;
      while (synt[++pos] == 'n');
      
      //Looking at a straight identifier. Make sure it actually needs declared.
      string nname = code.substr(spos,pos-spos);
      
      if (!nts)
        { pos--; continue; }
      
      //Decrement pos to avoid skipping a char on continue
      if (synt[pos--] != '(') // If it isn't a function (we assume it's nothing other than a function or varname)
      {
        if (in_decl and !dec_initializing)
        {
          if (!dec_name_givn) {
            dec_name_givn = true;
            dec_name = nname;
          } continue;
        }

        //If we're currently looking for a timeline variable, check if this is it.
        if (grab_tline_index) {
          cout << "  Potentially calls timeline `" << nname << "'\n";
          pev->myObj->tlines.insert(pair<string,int>(nname,1));
          grab_tline_index = false;
        }

        //Before ignoring globals/locals, check if we're setting the timeline index.
        if (nname == "timeline_index") {
          grab_tline_index = true;
        }
        
        //First, check shared locals to see if we already have one
        if (shared_object_locals.find(nname) != shared_object_locals.end()) {
          pev->myObj->globallocals[nname]++;
          if (scopes.back().is_with) {
            pos += 5;
            cout << "Add a self. before " << nname;
            code.insert(spos,"self.");
            synt.insert(spos,"nnnn.");
          }
          cout << "Ignoring `" << nname << "' because it's a shared local.\n"; continue;
        }
        
        //Second, check that it's not a global
        if (lang->global_exists(nname)) {
          cout << "Ignoring `" << nname << "' because it's a global.\n";
          continue;
        }
        
        //Next, make sure we're not specifically ignoring it
        map<string,int>::const_iterator exIt;
        size_t i=scopes.size()-1;
        for (std::vector<scope_level>::const_reverse_iterator it=scopes.rbegin(); it!=scopes.rend(); it++) {
          if ((exIt = it->ignore.find(nname)) != it->ignore.end()) {
            cout << "Ignoring `" << nname << "' because it's on the ignore stack for level " <<i << " since position " << exIt->second << ".\n";
            goto continue_2;
          }
          i--;
        }
        
        int argnum, iscr;
        iscr = sscanf(nname.c_str(),"argument%d",&argnum);
        if (iscr == 1)
        { //  not in a script or are but have exceeded arg number
          if (global_script_argument_count < argnum + 1)
            global_script_argument_count = argnum + 1;
          continue;
        }
        
        //Last, make sure we're not in a with.
        if (scopes.back().is_with)
        {
          pos += 5;
          code.insert(spos,"self.");
          synt.insert(spos,"nnnn.");
        }
        
        //Of course, we also don't want to risk overwriting a typed version
        if (pev->myObj->locals.find(nname) != pev->myObj->locals.end()) {
          cout << "Ignoring `" << nname << "' because it's already a local.\n"; continue;
        }
        
        cout << "Adding `" << nname << "' because that's just what I do.\n";
        pev->myObj->locals[nname] = dectrip();
        continue_2: continue;
      }
      else //Since a syntax check already completed, we assume this is a valid function
      {
        bool contented = false;
        unsigned pars = 1, args = 0;

        //If this is a specific action, we can actually grab timeline indices.
        if (nname == "action_set_timeline") {
            size_t nextSep = synt.find_first_not_of("n", pos+2);
            if (nextSep != std::string::npos) {
              const string pname = code.substr(pos+2,nextSep-(pos+2));
              cout << "  Potentially calls timeline `" << pname << "'\n";
              pev->myObj->tlines.insert(pair<string,int>(pname,1));
            }
        }

        //Another special case: try to inline script_execute().
        if (nname == "script_execute") {
            size_t nextSep = synt.find_first_not_of("n", pos+2);
            if (nextSep != std::string::npos) {
              const string pname = code.substr(pos+2,nextSep-(pos+2));
              if (script_names.find(pname)!=script_names.end()) {
                cout << "  script_execute() inlining `" << pname << "'\n";
                int off = synt[nextSep]==')' ? 0 : 1;
                code.replace(spos, nextSep-spos+off, pname+"(");
                synt.replace(spos, nextSep-spos+off, std::string(pname.size(),'n')+"(");
                pos = spos + pname.size() - 1;
                nname = pname;
              }
            }
        }
        
        for (pt i = pos+2; pars; i++) //Start after parenthesis at pos+1, loop until that parenthesis is closed
        {
          if (synt[i] == ',' and pars == 1) {
            args += contented;
            contented = false; continue;
          }
          if (synt[i] == ')') { //TODO: if (a,) is one arg according to ISO, move this before contented = true;
            pars--; continue;
          }
          contented = true;
          if (synt[i] == '(') {
            pars++; continue;
          }
        }
        args += contented; //Final arg for closing parentheses
        pair<parsed_object::funcit,bool> a = pev->myObj->funcs.insert(pair<string,int>(nname,args));
        if (!a.second and a.first->second < signed(args))
          a.first->second = args;
        cout << "  Calls script `" << nname << "'\n";
      }
    }
    else if (synt[pos] == 's')
    {
      const size_t sp = pos;
      while (synt[++pos] == 's');
      std::string stmt = code.substr(sp,(pos--)-sp);
      if (stmt=="with" || stmt=="if" || stmt=="for" || stmt=="while") {
        counting_parens = 1; //Prepare for a new scope (after parens, of course).
        last_was_with = stmt=="with";
      }
      continue;
    } 
    else if (synt[pos] == 'e')  //"else", or "elseif" (but "if" is still "ss").
    {
      //Only reify "else" here.
      const size_t sp = pos;
      while (synt[++pos] == 'e');
      std::string stmt = code.substr(sp,(pos--)-sp);
      if (stmt=="else") {
        //Handle "elseif" later.
        if (pos+1<synt.size() && synt[pos]!='s') {
          //Reify the scope immediately.
          scopes.push_back(reify_scope.second);
          reify_scope.first = false;

          //This may also be single-line.
          skip_next_bracket = pos+1<synt.size() && synt[pos+1]=='{';
          scopes.back().no_bracket = !skip_next_bracket;
        }
      } else {
        reify_scope.first = false; //TODO: Do we have any other "e" syntaxes except "else"?
      }

      //TODO: Reify scope; but delay if "elseif"
      //scopes.push_back(reify_scope);
    }
  }
  
  //cout << "**Finished collections in " << (pev==NULL ? "some event for some unspecified object" : pev->myObj->name + ", event " + event_get_human_name(pev->mainId,pev->id))<< "\n";
  
  //Store these for later.
  pev->code = code;
  pev->synt = synt;

std::cerr <<"AFTR: ###" <<code <<"###\n";
}
