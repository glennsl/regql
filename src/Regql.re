module type Network = {let uri: string; let token: string;};

module type Container = {
  type shape;
  type variables;
  let decoder: Js.Json.t => shape;
  let runQuery: CompTypes.query;
};

module Create = (Network: Network, Container: Container) => {
  module Component = {
    type shape = Container.shape;
    type variables = Container.variables;
    let decoder = Container.decoder;
    let runQuery = Container.runQuery;
    let uri = Network.uri;
    let token = Network.token;
  };
  let onMount: (module CompTypes.Sig) = (module OnMountComp.Create(Component));
  let onQuery: (module CompTypes.Sig) = (module OnQueryComp.Create(Component));
  switch Container.runQuery {
  | OnMount => onMount
  | OnQuery => onQuery
  };
};