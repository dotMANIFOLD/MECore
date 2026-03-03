namespace ME {
    abstract shared class Component {
        private Component() final {

        }
        Component(const Component &inout) delete;

        // Runs before this component's first update.
        protected void OnStart() { }

        // Runs every frame.
        protected void OnUpdate() { }
    }
}
