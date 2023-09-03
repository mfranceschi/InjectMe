import argparse
import string
from dataclasses import dataclass, field
from random import sample


class ClassNamesFactory:
    """
    Factory for unique class names.
    """

    def __init__(self):
        self.names: set[str] = set()

    def make_names(self, nbr_classes: int, length: int) -> set[str]:
        for _ in range(nbr_classes):
            self.names.add(self._make_unique_class_name(length))
        return self.names

    def _make_unique_class_name(self, length: int) -> str:
        class_name = self._make_class_name(length)
        while class_name in self.names:
            class_name = self._make_class_name(length)
        return class_name

    @staticmethod
    def _make_class_name(length: int) -> str:
        return "".join(sample(string.ascii_uppercase, length))


@dataclass
class ClassContents:
    name: str
    use_injector: bool = False

    injected_data_members: set[str] = field(default_factory=set)
    normal_data_members: set[str] = field(default_factory=set)

    @property
    def declaration(self):
        return f"class {self.name}; "

    @property
    def definition(self):
        injected_data_members_as_string = "\n".join(
            f"Injected<{theClass}> injected_{theClass} {{}}; " for theClass in self.injected_data_members)

        normal_data_members_as_string = "\n".join(
            f"{theClass}> normal_{theClass} {{}}; " for theClass in self.normal_data_members)

        canonical_form = f"""{self.name} () = default; 
    {self.name} (const {self.name}&) = default;
    {self.name} & operator = (const {self.name}&) = default;
    {self.name} ( {self.name} && ) = default; 
    {self.name} & operator= ( {self.name} && ) = default; 
    ~{self.name} () = default;"""

        return f"""class {self.name} {{ 
    public: 
        {canonical_form} 
    private:
        {injected_data_members_as_string}{normal_data_members_as_string}
}}; """

    @property
    def configuration_instruction(self):
        return f"""configure<{self.name}>().done(); """ if self.use_injector else ""

    @property
    def injection_instruction(self):
        f"""
    {{
        const auto injected = inject<{self.name}>();
        ((void)0)(injected);
    }}"""
        return f"inject<{self.name}>();"


@dataclass
class Program:
    declarations: list[str]
    definitions: list[str]
    configuration_instructions: list[str]
    injectorsToCall: list[str]

    INCLUDE_INJECTME: str = """#include "InjectMe.hpp" """
    USING_NAMESPACE_INJECTME: str = "using namespace mf::InjectMe;"
    NEWLINE = "\n"
    NEWLINE_WITH_TABS = "\n    "

    def __str__(self):
        return f"""
{self.INCLUDE_INJECTME}
{self.USING_NAMESPACE_INJECTME}

// Declarations
{self.NEWLINE.join(self.declarations)}

// Definitions
{(self.NEWLINE * 2).join(self.definitions)}

void configureInjections() {{
    {self.NEWLINE_WITH_TABS.join(self.configuration_instructions)}
}}

void callInjectors() {{
    {self.NEWLINE_WITH_TABS.join(self.injectorsToCall)}
}}

int main () {{
    configureInjections();
    callInjectors();
    return 0; 
}}
        """


def main():
    # Parse arguments, if any
    parser = argparse.ArgumentParser()
    parser.add_argument("--nbrClasses", help="How many classes to generate and configure?", type=int, default=10)
    parser.add_argument("--classNameLength", help="Length, in chars (ASCII uppercase), of the generated class names",
                        type=int, default=4)
    parser.add_argument("--nbrClassesToInject", help="Number of classes to use the injection on", type=int, default=-1)
    parser.add_argument("--targetFile", help="File to write to", type=str, default="")
    args = parser.parse_args()

    # Set up code generation: generate class names, etc.
    class_names = ClassNamesFactory().make_names(args.nbrClasses, args.classNameLength)
    classes: dict[str, ClassContents] = {class_name: ClassContents(name=class_name) for class_name in class_names}
    for class_content in sample(list(classes.values()), args.nbrClassesToInject if args.nbrClassesToInject >= 0 else args.nbrClasses):
        class_content.use_injector = True

    # Generate corresponding CPP code
    program = Program(
        declarations=[class_content.declaration for class_content in classes.values()],
        definitions=[class_content.definition for class_content in classes.values()],
        configuration_instructions=[class_content.configuration_instruction for class_content in classes.values()],
        injectorsToCall=[class_content.injection_instruction for class_content in classes.values() if class_content.injection_instruction]
    )
    if args.targetFile:
        with open(args.targetFile, "w+") as f:
            f.write(str(program))
    else:
        print(program)


main()
