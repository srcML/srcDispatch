/**
 * @file DeclTypePolicySingleEvent.hpp
 *
 * @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <srcSAXEventDispatchUtilities.hpp>

#include <TypePolicySingleEvent.hpp>
#include <NamePolicySingleEvent.hpp>

#include <string>
#include <vector>

#ifndef INCLUDED_DECL_TYPE_POLICY_SINGLE_EVENT_HPP
#define INCLUDED_DECL_TYPE_POLICY_SINGLE_EVENT_HPP

class DeclTypePolicy : public srcSAXEventDispatch::EventListener, public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener {

public:

    struct DeclTypeData {

        std::shared_ptr<TypePolicy::TypeData> type;
        NamePolicy::NameData * name;
        bool isStatic;

        friend std::ostream & operator<<(std::ostream & out, const DeclTypeData & declData) {

            out << *declData.type;

            if(declData.name)
                out << ' ' << *declData.name;

            return out;

        }

    };

private:

    std::vector<DeclTypeData *> data;
    std::size_t declDepth;

    TypePolicy * typePolicy;
    bool isStatic;
    std::shared_ptr<TypePolicy::TypeData> type;
 
    NamePolicy * namePolicy;

public:

    DeclTypePolicy(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners)
        : srcSAXEventDispatch::PolicyDispatcher(listeners),
          data{},
          declDepth(0),
          typePolicy(nullptr),
          isStatic(false),
          type(),
          namePolicy(nullptr) { 
    
        InitializeDeclTypePolicyHandlers();

    }

    ~DeclTypePolicy() {

        if(typePolicy) delete typePolicy;
        if(namePolicy) delete namePolicy;

    }

protected:
    void * DataInner() const override {

        return (void *)&data;

    }
    void NotifyWrite(const PolicyDispatcher * policy, srcSAXEventDispatch::srcSAXEventContext & ctx) override {} //doesn't use other parsers
    virtual void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {

        if(typeid(TypePolicy) == typeid(*policy)) {

            type = std::shared_ptr<TypePolicy::TypeData>(policy->Data<TypePolicy::TypeData>());
            ctx.dispatcher->RemoveListenerDispatch(nullptr);

        } else if(typeid(NamePolicy) == typeid(*policy)) {

            data.back()->name = policy->Data<NamePolicy::NameData>(); 
            ctx.dispatcher->RemoveListenerDispatch(nullptr);

        }

    }

private:

    void InitializeDeclTypePolicyHandlers() {
        using namespace srcSAXEventDispatch;

        // start of policy
        openEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {

            if(!declDepth) {

                declDepth = ctx.depth;

                CollectTypeHandlers();
                CollectNameHandlers();
                CollectSpecifiersHandlers();

            }

            openEventMap[ParserState::decl] = [this](srcSAXEventContext& ctx) {

                if(declDepth && (declDepth + 1) == ctx.depth) {
                    data.push_back(new DeclTypeData{});
                }

            };

            closeEventMap[ParserState::decl] = [this](srcSAXEventContext& ctx) {

                if(declDepth && (declDepth + 1) == ctx.depth) {
                    data.back()->isStatic = isStatic;
                    data.back()->type = type;
                }

            };

        };

        // end of policy
        closeEventMap[ParserState::declstmt] = [this](srcSAXEventContext& ctx) {

            if(declDepth && declDepth == ctx.depth) {

                declDepth = 0;
 
                NotifyAll(ctx);
                InitializeDeclTypePolicyHandlers();

            }
           
        };

    }

    void CollectTypeHandlers() {
        using namespace srcSAXEventDispatch;

        openEventMap[ParserState::type] = [this](srcSAXEventContext& ctx) {

            if(declDepth && (declDepth + 2) == ctx.depth) {

                if(!typePolicy) typePolicy = new TypePolicy{this};
                ctx.dispatcher->AddListenerDispatch(typePolicy);

            }

        };

    }

    void CollectNameHandlers() {
        using namespace srcSAXEventDispatch;

        openEventMap[ParserState::name] = [this](srcSAXEventContext& ctx) {

            if(declDepth && (declDepth + 2) == ctx.depth) {

                if(!namePolicy) namePolicy = new NamePolicy{this};
                ctx.dispatcher->AddListenerDispatch(namePolicy);

            }

        };

    }



void CollectSpecifiersHandlers() {
    using namespace srcSAXEventDispatch;

    openEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx) {

        if(declDepth && (declDepth + 2) == ctx.depth) {

            closeEventMap[ParserState::tokenstring] = [this](srcSAXEventContext& ctx) {

                if(ctx.currentToken == "static")
                    isStatic = true;

            };

        }

    };

    closeEventMap[ParserState::specifier] = [this](srcSAXEventContext& ctx) {

        if(declDepth && (declDepth + 1) == ctx.depth) {

            NopCloseEvents({ParserState::tokenstring});

        }

    };

}

};

#endif
